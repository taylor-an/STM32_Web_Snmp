#include "netif/ethernetif.h" 
#include "lan8720.h"  
#include "lwip_comm.h" 
#include "netif/etharp.h"  
#include "string.h"  
#include "snmp.h"
#include "stm32_eth.h"


static void ETH_PTPRxPkt_ChainMode(FrameTypeDef * frame);
static void ETH_PTPRxPkt_ChainMode_CleanUp(FrameTypeDef * frame, struct ptptime_t * timestamp);
static void ETH_RxPkt_ChainMode_CleanUp(FrameTypeDef * frame);
static u8 * ETH_PTPTxPkt_PrepareBuffer(void);
static u32 ETH_PTPTxPkt_ChainMode(u16 FrameLength, struct ptptime_t * timestamp);


//��ethernetif_init()�������ڳ�ʼ��Ӳ��
//netif:�����ṹ��ָ�� 
//����ֵ:ERR_OK,����
//       ����,ʧ��
static err_t low_level_init(struct netif *netif)
{
#ifdef CHECKSUM_BY_HARDWARE
	int i; 
#endif 
	netif->hwaddr_len = ETHARP_HWADDR_LEN; //����MAC��ַ����,Ϊ6���ֽ�
	//��ʼ��MAC��ַ,����ʲô��ַ���û��Լ�����,���ǲ����������������豸MAC��ַ�ظ�
	netif->hwaddr[0]=lwipdev.mac[0]; 
	netif->hwaddr[1]=lwipdev.mac[1]; 
	netif->hwaddr[2]=lwipdev.mac[2];
	netif->hwaddr[3]=lwipdev.mac[3];
	netif->hwaddr[4]=lwipdev.mac[4];
	netif->hwaddr[5]=lwipdev.mac[5];
	netif->mtu=1500; //��������䵥Ԫ,����������㲥��ARP����

	netif->flags = NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;
	
	ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); //��STM32F4��MAC��ַ�Ĵ�����д��MAC��ַ
	ETH_DMAPTPTxDescChainInit(DMATxDscrTab, DMAPTPTxDscrTab, Tx_Buff, ETH_TXBUFNB);
	ETH_DMAPTPRxDescChainInit(DMARxDscrTab, DMAPTPRxDscrTab,  Rx_Buff, ETH_RXBUFNB);
#ifdef CHECKSUM_BY_HARDWARE 	//ʹ��Ӳ��֡У��
	for(i=0;i<ETH_TXBUFNB;i++)	//ʹ��TCP,UDP��ICMP�ķ���֡У��,TCP,UDP��ICMP�Ľ���֡У����DMA��������
	{
		ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
	}
#endif
	ETH_Start(); //����MAC��DMA				
	return ERR_OK;
} 
//���ڷ������ݰ�����ײ㺯��(lwipͨ��netif->linkoutputָ��ú���)
//netif:�����ṹ��ָ��
//p:pbuf���ݽṹ��ָ��
//����ֵ:ERR_OK,��������
//       ERR_MEM,����ʧ��
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	u8 res;
	struct pbuf *q;
	int l = 0;
	u8 *buffer;
	
  struct ptptime_t timestamp;
  buffer = ETH_PTPTxPkt_PrepareBuffer();
	
	 if(buffer == NULL) {
    res = ETH_ERROR; /* ERR_MEM */
  }
	 
    for(q = p; q != NULL; q = q->next)
  {
    memcpy((u8_t*)&buffer[l], q->payload, q->len);
    l = l + q->len;
  }
  
	  if( ETH_SUCCESS == ETH_PTPTxPkt_ChainMode(l, &timestamp) ) {

  } else {
    res = ETH_ERROR;
  }
	
	if(res==ETH_ERROR)
	{
		snmp_inc_ifoutdiscards(netif);
		return ERR_MEM;//���ش���״̬
	}
	else
	{
		snmp_add_ifoutoctets(netif, l);
	}
	
	return ERR_OK;
}

///���ڽ������ݰ�����ײ㺯��
//neitif:�����ṹ��ָ��
//����ֵ:pbuf���ݽṹ��ָ��
static struct pbuf * low_level_input(struct netif *netif)
{  
	struct pbuf *p, *q;
	u32_t len;
	int l =0;
	FrameTypeDef frame;
	u8 *buffer;
	struct ptptime_t timestamp;
	
	p = NULL;	
	frame.descriptor = NULL;	
    frame.PTPdescriptor = NULL;
    ETH_PTPRxPkt_ChainMode(&frame);
	
	 len = frame.length;
	buffer = (u8 *)frame.buffer;
	 p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);	
		  
	 if(p!=NULL)
	 {
		for(q=p;q!=NULL;q=q->next)
		{
			memcpy((u8_t*)q->payload, (u8_t*)&buffer[l], q->len);
			l=l+q->len;
		}    
	 }
	  else
	  {
		  snmp_add_ifinoctets(netif, len);
	  }
		
	 ETH_PTPRxPkt_ChainMode_CleanUp(&frame, &timestamp);
	ETH_RxPkt_ChainMode_CleanUp(&frame);
	return p;
} 




//������������(lwipֱ�ӵ���)
//netif:�����ṹ��ָ��
//����ֵ:ERR_OK,��������
//       ERR_MEM,����ʧ��
err_t ethernetif_input(struct netif *netif)
{
	err_t err;
	struct pbuf *p;
	p=low_level_input(netif);
	if(p==NULL) return ERR_MEM;
	err=netif->input(p, netif);
	if(err!=ERR_OK)
	{
		LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_input: IP input error\n"));
		pbuf_free(p);
		p = NULL;
	} 
	return err;
} 
//ʹ��low_level_init()��������ʼ������
//netif:�����ṹ��ָ��
//����ֵ:ERR_OK,����
//       ����,ʧ��
err_t ethernetif_init(struct netif *netif)
{
	LWIP_ASSERT("netif!=NULL",(netif!=NULL));
#if LWIP_NETIF_HOSTNAME			//LWIP_NETIF_HOSTNAME 
	netif->hostname="lwip";  	//��ʼ������
#endif 

#if LWIP_SNMP
	netif->link_type = 1;
	netif->link_speed = 0;
	netif->ts         = 0;
	netif->ifinoctets = 0;
  netif->ifinucastpkts = 0;
  netif->ifinnucastpkts = 0;
  netif->ifindiscards = 0;
  netif->ifoutoctets = 0;
  netif->ifoutucastpkts = 0;
  netif->ifoutnucastpkts = 0;
  netif->ifoutdiscards = 0;
#endif
	
	netif->name[0]=IFNAME0; 	//��ʼ������netif��name�ֶ�
	netif->name[1]=IFNAME1; 	//���ļ��ⶨ�����ﲻ�ù��ľ���ֵ
	netif->output=etharp_output;//IP�㷢�����ݰ�����
	netif->linkoutput=low_level_output;//ARPģ�鷢�����ݰ�����
	low_level_init(netif); 		//�ײ�Ӳ����ʼ������
	return ERR_OK;
}



static void ETH_PTPRxPkt_ChainMode(FrameTypeDef * frame)
{
  u32 framelength = 0;
  frame->length = 0;
  frame->buffer = 0;

  /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
  if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET)
  {
    frame->length = ETH_ERROR;

    if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET)
    {
      /* Clear RBUS ETHERNET DMA flag */
      ETH->DMASR = ETH_DMASR_RBUS;
      /* Resume DMA reception */
      ETH->DMARPDR = 0;
    }

    /* Return error: OWN bit set */
    return;
  }

  if(((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (u32)RESET) &&
     ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (u32)RESET) &&
     ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (u32)RESET))
  {
    /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
    framelength = ((DMARxDescToGet->Status & ETH_DMARxDesc_FL) >> ETH_DMARxDesc_FrameLengthShift) - 4;

    /* Get the addrees of the actual buffer */
    frame->buffer = DMAPTPRxDescToGet->Buffer1Addr;
  }
  else
  {
    /* Return ERROR */
    framelength = ETH_ERROR;
  }

  frame->length = framelength;


  frame->descriptor = DMARxDescToGet;
  frame->PTPdescriptor = DMAPTPRxDescToGet;

  /* Update the ETHERNET DMA global Rx descriptor with next Rx decriptor */
  /* Chained Mode */
  /* Selects the next DMA Rx descriptor list for next buffer to read */
  DMARxDescToGet = (ETH_DMADESCTypeDef*) (DMAPTPRxDescToGet->Buffer2NextDescAddr);
  if(DMAPTPRxDescToGet->Status != 0)
  {
    DMAPTPRxDescToGet = (ETH_DMADESCTypeDef*) (DMAPTPRxDescToGet->Status);
  }
  else
  {
    DMAPTPRxDescToGet++;
  }

  /* Return Frame */
  return;
}

u32_t ETH_PTPSubSecond2NanoSecondABC(u32_t SubSecondValue)
{
  uint64_t val = SubSecondValue * 1000000000ll;
  val >>=31;
  return val;
}

static void ETH_PTPRxPkt_ChainMode_CleanUp(FrameTypeDef * frame, struct ptptime_t * timestamp)
{
  timestamp->tv_nsec = ETH_PTPSubSecond2NanoSecondABC(frame->descriptor->Buffer1Addr);
  timestamp->tv_sec = frame->descriptor->Buffer2NextDescAddr;

  frame->descriptor->Buffer1Addr = frame->PTPdescriptor->Buffer1Addr;
  frame->descriptor->Buffer2NextDescAddr = frame->PTPdescriptor->Buffer2NextDescAddr;
}

static void ETH_RxPkt_ChainMode_CleanUp(FrameTypeDef * frame)
{
  /* Set Own bit of the Rx descriptor Status: gives the buffer back to ETHERNET DMA */
  frame->descriptor->Status = ETH_DMARxDesc_OWN;

  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET)
  {
    /* Clear RBUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    ETH->DMARPDR = 0;
  }
}

static u8 * ETH_PTPTxPkt_PrepareBuffer() {
  /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
  if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
  {
    /* Return ERROR: OWN bit set */
    return NULL;
  }

  DMATxDescToSet->Buffer1Addr = DMAPTPTxDescToSet->Buffer1Addr;
  DMATxDescToSet->Buffer2NextDescAddr = DMAPTPTxDescToSet->Buffer2NextDescAddr;

  return (u8 *) ETH_GetCurrentTxBuffer();
}


static u32 ETH_PTPTxPkt_ChainMode(u16 FrameLength, struct ptptime_t * timestamp)
{
  unsigned int timeout = 0;

  /* Setting the Frame Length: bits[12:0] */
  DMATxDescToSet->ControlBufferSize = (FrameLength & ETH_DMATxDesc_TBS1);

  /* Setting the last segment and first segment bits (in this case a frame is transmitted in one descriptor) */
  DMATxDescToSet->Status |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;

  /* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
  DMATxDescToSet->Status |= ETH_DMATxDesc_OWN;

  /* When Tx Buffer unavailable flag is set: clear it and resume transmission */
  if ((ETH->DMASR & ETH_DMASR_TBUS) != (u32)RESET)
  {
    /* Clear TBUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_TBUS;
    /* Resume DMA transmission*/
    ETH->DMATPDR = 0;
  }

  /* Wait for ETH_DMATxDesc_TTSS flag to be set */
  do
  {
    timeout++;
  } while (!(DMATxDescToSet->Status & ETH_DMATxDesc_TTSS) && (timeout < PHY_READ_TO));
  /* Return ERROR in case of timeout */
  if(timeout == PHY_READ_TO)
  {
    return ETH_ERROR;
  }

  timestamp->tv_nsec = ETH_PTPSubSecond2NanoSecondABC(DMATxDescToSet->Buffer1Addr);
  timestamp->tv_sec = DMATxDescToSet->Buffer2NextDescAddr;

  /* Clear the DMATxDescToSet status register TTSS flag */
  DMATxDescToSet->Status &= ~ETH_DMATxDesc_TTSS;

  DMATxDescToSet->Buffer1Addr = DMAPTPTxDescToSet->Buffer1Addr;
  DMATxDescToSet->Buffer2NextDescAddr = DMAPTPTxDescToSet->Buffer2NextDescAddr;

  /* Update the ETHERNET DMA global Tx descriptor with next Tx decriptor */
  /* Chained Mode */
  /* Selects the next DMA Tx descriptor list for next buffer to send */
  DMATxDescToSet = (ETH_DMADESCTypeDef*) (DMATxDescToSet->Buffer2NextDescAddr);

  if(DMAPTPTxDescToSet->Status != 0)
  {
    DMAPTPTxDescToSet = (ETH_DMADESCTypeDef*) (DMAPTPTxDescToSet->Status);
  }
  else
  {
    DMAPTPTxDescToSet++;
  }

  /* Return SUCCESS */
  return ETH_SUCCESS;
}

