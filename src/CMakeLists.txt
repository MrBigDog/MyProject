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

#uniscope
FOREACH( uniscope
		usXml
		usCore
		usUtil
		usFilter
		usLua
		usDotx
		usMeshop
		usGfx
		 )
ADD_SUBDIRECTORY(${uniscope})
SET_PROPERTY(TARGET ${uniscope} PROPERTY FOLDER "Uniscope")
ENDFOREACH( uniscope )


ADD_SUBDIRECTORY( gwDrivers )#插件
ADD_SUBDIRECTORY( examples ) #例子
#ADD_SUBDIRECTORY( Tools )   #工具