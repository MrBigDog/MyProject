#ifndef GWPIPELINE_PIPELINEDECLARE_H
#define GWPIPELINE_PIPELINEDECLARE_H 1

//////////////////////////////////////////////////////////////////////////
// 定义管网类型
// 电力
#define		PIPELINE_ELECTRICITY		0
// 电信
#define		PIPELINE_TELEGRAPHY			1
// 供水
#define		PIPELINE_FEEDWATER			2
// 排水
#define		PIPELINE_SEWAGE				3
// 燃气
#define		PIPELINE_GAS				4
// 热力
#define		PIPELINE_ENERGETICS			5
// 工业
#define		PIPELINE_INDUSTRY			6
// 电视
#define		PIPELINE_TELEVISION			7

// 管网类型
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 定义埋设方式

// 直埋
#define		PIPELINE_LAYOUT_ZHIMAI		0
// 管埋
#define		PIPELINE_LAYOUT_GUANMAI		1
// 管块
#define		PIPELINE_LAYOUT_GUANKUAI	2
// 沟道
#define		PIPELINE_LAYOUT_GOUDAO		3
// 架空
#define		PIPELINE_LAYOUT_JIAKONG		4

// 埋设方式
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 定义流向值域

// 未知
#define		PIPELINE_FLOW_UNKNOWN		0

// 起点到终点
#define		PIPELINE_FLOW_START_END		1

// 终点到起点
#define		PIPELINE_FLOW_END_START		2

// 双向
#define		PIPELINE_FLOW_DOUBLE		3

// 流向值域
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 定义容器材质

// 铸铁
#define		PIPELINE_MATERIAL_CAST_IRON				0

// 球墨铸铁
#define		PIPELINE_MATERIAL_NODULAR_CAST_IRON		1

// 砼
#define		PIPELINE_MATERIAL_BETON					2

// 钢
#define		PIPELINE_MATERIAL_STEEL					3

// 玻璃钢
#define		PIPELINE_MATERIAL_GFRP					4

// PVC
#define		PIPELINE_MATERIAL_PVC					5

// 陶瓷
#define		PIPELINE_MATERIAL_CERAMIC				6

// 砖
#define		PIPELINE_MATERIAL_BRICK					7

// 铜
#define		PIPELINE_MATERIAL_COPPER				8

// 光纤
#define		PIPELINE_MATERIAL_FIBER					9

// 未知
#define		PIPELINE_MATERIAL_UNKNOWN				0xffff

// 容器材质
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 定义点类型 0xffffffff
// 第4位代表类型，普通节点为0，连接点为1，功能设备点为2

//////////////////////////////////////////////////////////////////////////
// 普通点标记
#define PIPELINE_POINT_NORMAL					0x0000

// 非普查
#define PIPELINE_POINT_NONCENSUS				0x0001

// 预留口
#define PIPELINE_POINT_LEAVE_ORA				0x0002

// 直线点
#define PIPELINE_POINT_BEELINE_NODE				0x0003

// 转折点
#define PIPELINE_POINT_TURN_NODE				0x0004

// 分支点
#define PIPELINE_POINT_FILIATION				0x0005

// 变径点
#define PIPELINE_POINT_DIAMETER_CHANGE			0x0006

// 变深点
#define PIPELINE_POINT_DEPTH_CHANGE				0x0007

// 盖堵
#define PIPELINE_POINT_COVER					0x0008

// 源点
#define PIPELINE_POINT_SOURCE					0x0009

// 进水口
#define PIPELINE_POINT_INFALL					0x000A

// 出水口
#define PIPELINE_POINT_OUTLET					0x000B
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 连接点标记
#define PIPELINE_POINT_CONNECTION				0x1000

// 管帽
#define PIPELINE_POINT_CAP						0x1001

// 弯头
#define PIPELINE_POINT_ELBOW					0x1002

// 三通
#define PIPELINE_POINT_TEE						0x1003

// 四通
#define PIPELINE_POINT_DOUBLE_TEE				0x1004

// 五通
#define PIPELINE_POINT_BOTTOM_BRACKET			0x1005
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 设备点标记
#define PIPELINE_POINT_EQUIPMENT				0x2000

// 上杆
#define PIPELINE_POINT_UPPER_BOOM				0x2001

// 接线箱
#define PIPELINE_POINT_JOINT_BOX				0x2002

// 化粪池
#define PIPELINE_POINT_CESSPOOL					0x2003

// 阀门
#define PIPELINE_POINT_VALVE					0x2004

// 水表
#define PIPELINE_POINT_WATER_METER				0x2005

// 消防栓
#define PIPELINE_POINT_FIREPLUG					0x2006

// 凝水缸
#define PIPELINE_POINT_CONDENSER_BOX			0x2007

// 调压箱
#define PIPELINE_POINT_PRB						0x2008
//////////////////////////////////////////////////////////////////////////

// 点类型
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 定义井形状

// 无井
#define		WELL_SHAPE_NONE			0

// 圆井
#define		WELL_SHAPE_CIRCLE		1
	
// 方井
#define		WELL_SHAPE_SQUARE		2

// 篦子
#define		WELL_SHAPE_GULLY		3

// 手孔
#define		WELL_SHAPE_MAN_HOLE		4

// 井形状
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 定义井类型


// 井类型
//////////////////////////////////////////////////////////////////////////


#endif // GWPIPELINE_PIPELINEDECLARE_H
