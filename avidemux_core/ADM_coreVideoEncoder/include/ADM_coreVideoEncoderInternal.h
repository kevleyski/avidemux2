/**
    \file  ADM_coreVideoEncoderInternal.h
    \brief interface to video encoder plugins

*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef VIDEOENCODERINTERNAL_H
#define VIDEOENCODERINTERNAL_H

#define ADM_VIDEO_ENCODER_API_VERSION 2
#include "ADM_coreVideoEncoder.h"
#include "DIA_uiTypes.h"
#include "ADM_paramList.h"

/*!
  This structure defines a video encoder
  \param encoder Encoder attached to this descriptor
  \param name The name of the codec
  \param configure Function to call to configure the codec
  \param param : An opaque structure that contains the codec specific configuration datas
*/
typedef struct
{
    const char   *encoderName;        // Internal name (tag)
    const char   *menuName;         // Displayed name (in menu)
    const char   *description;      // Short description

    uint32_t     apiVersion;            // const
    ADM_coreVideoEncoder *(*create)(ADM_coreVideoFilter *head);  
    void         (*destroy)(ADM_coreVideoEncoder *codec);
    bool         (*configure)(void);                                // Call UI to set it up
    bool         (*getConfigurationData)(CONFcouple **c); // Get the encoder private conf
    bool         (*setConfigurationData)(CONFcouple *c);   // Set the encoder private conf

    ADM_UI_TYPE  UIType;                // Type of UI
    uint32_t     major,minor,patch;     // Version of the plugin

    void         *opaque;               // Hide stuff in here
}ADM_videoEncoderDesc;

// Macros to declare audio encoder
/**************************************************************************/
#define ADM_DECLARE_VIDEO_ENCODER_PREAMBLE(Class) \
static bool getConfigurationData (CONFcouple **c); \
static bool setConfigurationData (CONFcouple *c);\
\
static ADM_coreVideoEncoder * create (ADM_coreVideoFilter * head) \
{ \
  return new Class (head); \
} \
static void destroy (ADM_coreVideoEncoder * in) \
{\
  Class *z = (Class *) in; \
  delete z; \
} 
//******************************************************

#define ADM_DECLARE_VIDEO_ENCODER_MAIN(name,menuName,desc,configure,uiType,maj,minV,patch,confTemplate,confVar) \
static ADM_videoEncoderDesc encoderDesc={\
    name,\
    menuName,\
    desc,\
    ADM_VIDEO_ENCODER_API_VERSION,\
    &create,\
    &destroy,\
    configure,\
    getConfigurationData,\
    setConfigurationData,\
    uiType,\
    maj,minV,patch,\
    NULL\
};\
bool getConfigurationData (CONFcouple **c)\
{\
         if(confTemplate==NULL) {*c=NULL;return true;} \
         return ADM_paramSave(c,confTemplate,confVar); \
}\
bool setConfigurationData (CONFcouple *c)\
{\
        return ADM_paramLoad(c,confTemplate,confVar); \
} \
extern "C" ADM_videoEncoderDesc *getInfo (void) \
{ \
  return &encoderDesc; \
}  \

#ifndef QT_TR_NOOP
#define QT_TR_NOOP(x) x
#endif
#endif
//EOF
