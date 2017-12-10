//#include <gwPipeline/PipelineSourceUtil.h>
//#include <gwPipeline/PipeEquipmentSymbol.h>
//#include <gwPipeline/PipeJointSymbol.h>
//#include <gwPipeline/PipeLineSymbol.h>
//#include <gwPipeline/PipelineResetAltFilter.h>
////#include <mdPipeline/WFSPipelineOptions.h>
//#include <gwEarth/tileSourceUtil.h>
//
//#include <osgDB/FileNameUtils>
//
////using namespace mdEarth;
//
//namespace gwPipeline
//{
//	PipelineSourceUtils::PipelineSourceUtils()
//	{
//
//	}
//
//	bool PipelineSourceUtils::createPipelineSourceOptions(const MDPropertyNode* args, 
//		mdEarth::Driver::FeaturePipelineModelOptions& out_opt)
//	{
//		//获取features数据;
//		osgEarth::Features::FeatureSourceOptions fsoptions;
//		if (!SourceUtil::createFeatureSourceOptions(args, fsoptions))
//		{
//			return false;
//		}
//
//		//WFSPipelineOptions fsoptions;
//		//fsoptions.url() = "http://10.168.17.64:8080/zy-server/wks_kunming_pipeline/ows";
//		//fsoptions.typeName() = "wks_kunming_pipeline:km_ysl";
//		//fsoptions.outputFormat() = "JSON";
//
//		ResetAltFilterOptions resetaltopt;
//		resetaltopt.type() = ResetAltFilterOptions::RESET_CONTAINER;
//		fsoptions.filters().push_back(resetaltopt);
//
//		std::string style_name = args->getStringValue("style_name", "pipeline_style");
//
//		osgEarth::Features::Style style;
//		style.setName(style_name);
//
//		std::string symboloType = osgDB::convertToLowerCase(args->getStringValue("symbol_type"));
//		if (symboloType=="container")
//		{
//			PipeLineSymbol* pipeline = style.getOrCreate<PipeLineSymbol>();
//			pipeline->setName("unit pipe");
//			//pipeline->setUnitPipe(createUnitCylinder(17));
//			pipeline->url()->setLiteral("E:\\BYL\\dogearth\\osgEarthWfs\\unitPipe.ive");
//		}
//		else if (symboloType=="joint")
//		{
//			PipeJointSymbol* jointSymbol =style.getOrCreate<PipeJointSymbol>();
//		}
//		else if (symboloType=="well")
//		{
//
//		}
//		else if (symboloType=="equipment")
//		{
//			PipeEquipmentSymbol* equipmentSymbol = style.getOrCreate<PipeEquipmentSymbol>();
//		}
//
//		float minrange = args->getFloatValue("min_range", 0.0f);
//		float maxrange = args->getFloatValue("max_range", FLT_MAX);
//		float sizefactor = args->getFloatValue("size_factor", 15);
//		osgEarth::Features::FeatureDisplayLayout layout;
//		layout.tileSizeFactor() = sizefactor;
//		layout.addLevel(osgEarth::Features::FeatureLevel(minrange, maxrange, style_name));
//
//		mdEarth::Driver::FeaturePipelineModelOptions opt;
//		opt.featureOptions() = fsoptions;
//		opt.styles() = new osgEarth::Features::StyleSheet();
//		opt.styles()->addStyle(style);
//		opt.layout() = layout;
//		opt.clusterCulling()= false;
//		opt.enableLighting()= false;
//		opt.backfaceCulling()= false;
//		opt.featureIndexing()->enabled() = true;// = true;
//		opt.featureIndexing()->embedFeatures()= false;
//		out_opt = opt;
//
//		return true;
//	}
//}