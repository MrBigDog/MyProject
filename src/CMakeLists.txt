#osgEarth扩展功能
FOREACH( osgEarthExt
		 osgEarthFeaturesExt
		 osgEarthSymbologyExt
		 )
ADD_SUBDIRECTORY(${osgEarthExt})
SET_PROPERTY(TARGET ${osgEarthExt} PROPERTY FOLDER "osgEarthExt")
ENDFOREACH( osgEarthExt )

#核心功能
FOREACH( coreLib
		 gwBase
		 gwEarth
		 gwApp	 
         gwUtil
		 gwTool
		 )
ADD_SUBDIRECTORY(${coreLib})
SET_PROPERTY(TARGET ${coreLib} PROPERTY FOLDER "Core")
ENDFOREACH( coreLib )


ADD_SUBDIRECTORY( gwDrivers )#插件
ADD_SUBDIRECTORY( examples ) #例子
#ADD_SUBDIRECTORY( Tools )   #工具