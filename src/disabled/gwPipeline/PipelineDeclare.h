#ifndef GWPIPELINE_PIPELINEDECLARE_H
#define GWPIPELINE_PIPELINEDECLARE_H 1

//////////////////////////////////////////////////////////////////////////
// �����������
// ����
#define		PIPELINE_ELECTRICITY		0
// ����
#define		PIPELINE_TELEGRAPHY			1
// ��ˮ
#define		PIPELINE_FEEDWATER			2
// ��ˮ
#define		PIPELINE_SEWAGE				3
// ȼ��
#define		PIPELINE_GAS				4
// ����
#define		PIPELINE_ENERGETICS			5
// ��ҵ
#define		PIPELINE_INDUSTRY			6
// ����
#define		PIPELINE_TELEVISION			7

// ��������
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// �������跽ʽ

// ֱ��
#define		PIPELINE_LAYOUT_ZHIMAI		0
// ����
#define		PIPELINE_LAYOUT_GUANMAI		1
// �ܿ�
#define		PIPELINE_LAYOUT_GUANKUAI	2
// ����
#define		PIPELINE_LAYOUT_GOUDAO		3
// �ܿ�
#define		PIPELINE_LAYOUT_JIAKONG		4

// ���跽ʽ
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ��������ֵ��

// δ֪
#define		PIPELINE_FLOW_UNKNOWN		0

// ��㵽�յ�
#define		PIPELINE_FLOW_START_END		1

// �յ㵽���
#define		PIPELINE_FLOW_END_START		2

// ˫��
#define		PIPELINE_FLOW_DOUBLE		3

// ����ֵ��
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ������������

// ����
#define		PIPELINE_MATERIAL_CAST_IRON				0

// ��ī����
#define		PIPELINE_MATERIAL_NODULAR_CAST_IRON		1

// ��
#define		PIPELINE_MATERIAL_BETON					2

// ��
#define		PIPELINE_MATERIAL_STEEL					3

// ������
#define		PIPELINE_MATERIAL_GFRP					4

// PVC
#define		PIPELINE_MATERIAL_PVC					5

// �մ�
#define		PIPELINE_MATERIAL_CERAMIC				6

// ש
#define		PIPELINE_MATERIAL_BRICK					7

// ͭ
#define		PIPELINE_MATERIAL_COPPER				8

// ����
#define		PIPELINE_MATERIAL_FIBER					9

// δ֪
#define		PIPELINE_MATERIAL_UNKNOWN				0xffff

// ��������
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ��������� 0xffffffff
// ��4λ�������ͣ���ͨ�ڵ�Ϊ0�����ӵ�Ϊ1�������豸��Ϊ2

//////////////////////////////////////////////////////////////////////////
// ��ͨ����
#define PIPELINE_POINT_NORMAL					0x0000

// ���ղ�
#define PIPELINE_POINT_NONCENSUS				0x0001

// Ԥ����
#define PIPELINE_POINT_LEAVE_ORA				0x0002

// ֱ�ߵ�
#define PIPELINE_POINT_BEELINE_NODE				0x0003

// ת�۵�
#define PIPELINE_POINT_TURN_NODE				0x0004

// ��֧��
#define PIPELINE_POINT_FILIATION				0x0005

// �侶��
#define PIPELINE_POINT_DIAMETER_CHANGE			0x0006

// �����
#define PIPELINE_POINT_DEPTH_CHANGE				0x0007

// �Ƕ�
#define PIPELINE_POINT_COVER					0x0008

// Դ��
#define PIPELINE_POINT_SOURCE					0x0009

// ��ˮ��
#define PIPELINE_POINT_INFALL					0x000A

// ��ˮ��
#define PIPELINE_POINT_OUTLET					0x000B
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ���ӵ���
#define PIPELINE_POINT_CONNECTION				0x1000

// ��ñ
#define PIPELINE_POINT_CAP						0x1001

// ��ͷ
#define PIPELINE_POINT_ELBOW					0x1002

// ��ͨ
#define PIPELINE_POINT_TEE						0x1003

// ��ͨ
#define PIPELINE_POINT_DOUBLE_TEE				0x1004

// ��ͨ
#define PIPELINE_POINT_BOTTOM_BRACKET			0x1005
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �豸����
#define PIPELINE_POINT_EQUIPMENT				0x2000

// �ϸ�
#define PIPELINE_POINT_UPPER_BOOM				0x2001

// ������
#define PIPELINE_POINT_JOINT_BOX				0x2002

// �����
#define PIPELINE_POINT_CESSPOOL					0x2003

// ����
#define PIPELINE_POINT_VALVE					0x2004

// ˮ��
#define PIPELINE_POINT_WATER_METER				0x2005

// ����˨
#define PIPELINE_POINT_FIREPLUG					0x2006

// ��ˮ��
#define PIPELINE_POINT_CONDENSER_BOX			0x2007

// ��ѹ��
#define PIPELINE_POINT_PRB						0x2008
//////////////////////////////////////////////////////////////////////////

// ������
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ���微��״

// �޾�
#define		WELL_SHAPE_NONE			0

// Բ��
#define		WELL_SHAPE_CIRCLE		1
	
// ����
#define		WELL_SHAPE_SQUARE		2

// ����
#define		WELL_SHAPE_GULLY		3

// �ֿ�
#define		WELL_SHAPE_MAN_HOLE		4

// ����״
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ���微����


// ������
//////////////////////////////////////////////////////////////////////////


#endif // GWPIPELINE_PIPELINEDECLARE_H
