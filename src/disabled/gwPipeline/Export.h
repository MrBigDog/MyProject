#ifndef GWPIPELINE_EXPORT_H
#define GWPIPELINE_EXPORT_H 1

#pragma warning( disable : 4244 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4275 )
#pragma warning( disable : 4290 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4996 )

#define HYCOMMONWINAPI_EXPORTS
#ifdef  HYCOMMONWINAPI_EXPORTS
#define GWPIPELINE_EXPORT __declspec(dllexport)
#else
#define GWPIPELINE_EXPORT __declspec(dllimport)
#endif

#endif