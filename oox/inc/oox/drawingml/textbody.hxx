/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef OOX_DRAWINGML_TEXTBODY_HXX
#define OOX_DRAWINGML_TEXTBODY_HXX

#include "oox/drawingml/textbodyproperties.hxx"
#include "oox/drawingml/textliststyle.hxx"

namespace com { namespace sun { namespace star {
    namespace text { class XText; }
    namespace text { class XTextCursor; }
} } }

namespace oox { namespace core { class XmlFilterBase; } }

namespace oox { namespace drawingml {

class TextParagraph;
typedef RefVector< TextParagraph > TextParagraphVector;

class TextBody
{
public:
	TextBody();
	~TextBody();

    inline const TextParagraphVector&   getParagraphs() const { return maParagraphs; }
    TextParagraph&                      addParagraph();

    inline const TextListStyle&         getTextListStyle() const { return maTextListStyle; }
    inline TextListStyle&               getTextListStyle() { return maTextListStyle; }

    inline const TextBodyProperties&    getTextProperties() const { return maTextProperties; }
    inline TextBodyProperties&          getTextProperties() { return maTextProperties; }

	/** insert the text body at the text cursor */
    void                insertAt(
                            const ::oox::core::XmlFilterBase& rFilterBase,
                            const ::com::sun::star::uno::Reference < ::com::sun::star::text::XText > & xText,
                            const ::com::sun::star::uno::Reference < ::com::sun::star::text::XTextCursor > & xAt,
                            const TextCharacterProperties& rTextStyleProperties,
                            const TextListStylePtr& pMasterTextListStyle ) const;
protected:
    TextParagraphVector maParagraphs;
    TextBodyProperties  maTextProperties;
    TextListStyle       maTextListStyle;
};

} }

#endif  //  OOX_DRAWINGML_TEXTBODY_HXX
