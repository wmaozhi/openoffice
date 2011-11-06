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



#include <uievent.hxx>

namespace DOM { namespace events
{
    CUIEvent::CUIEvent()
        : CUIEvent_Base()
        , m_detail(0)
    {
    }

    Reference< XAbstractView > SAL_CALL
    CUIEvent::getView() throw(RuntimeException)
    {
        ::osl::MutexGuard const g(m_Mutex);
        return m_view;
    }

    sal_Int32 SAL_CALL CUIEvent::getDetail() throw(RuntimeException)
    {
        ::osl::MutexGuard const g(m_Mutex);
        return m_detail;
    }

    void SAL_CALL CUIEvent::initUIEvent(const OUString& typeArg,
                     sal_Bool canBubbleArg,
                     sal_Bool cancelableArg,
                     const Reference< XAbstractView >& viewArg,
                     sal_Int32 detailArg) throw(RuntimeException)
    {
        ::osl::MutexGuard const g(m_Mutex);

        CEvent::initEvent(typeArg, canBubbleArg, cancelableArg);
        m_view = viewArg;
        m_detail = detailArg;
    }


    // delegate to CEvent, since we are inheriting from CEvent and XEvent
    OUString SAL_CALL CUIEvent::getType() throw (RuntimeException)
    {
        return CEvent::getType();
    }

    Reference< XEventTarget > SAL_CALL CUIEvent::getTarget() throw (RuntimeException)
    {
        return CEvent::getTarget();
    }

    Reference< XEventTarget > SAL_CALL CUIEvent::getCurrentTarget() throw (RuntimeException)
    {
        return CEvent::getCurrentTarget();
    }

    PhaseType SAL_CALL CUIEvent::getEventPhase() throw (RuntimeException)
    {
        return CEvent::getEventPhase();
    }

    sal_Bool SAL_CALL CUIEvent::getBubbles() throw (RuntimeException)
    {
        return CEvent::getBubbles();
    }

    sal_Bool SAL_CALL CUIEvent::getCancelable() throw (RuntimeException)
    {
        // mutation events cannot be canceled
        return sal_False;
    }

    com::sun::star::util::Time SAL_CALL CUIEvent::getTimeStamp() throw (RuntimeException)
    {
        return CEvent::getTimeStamp();
    }

    void SAL_CALL CUIEvent::stopPropagation() throw (RuntimeException)
    {
        CEvent::stopPropagation();
    }
    void SAL_CALL CUIEvent::preventDefault() throw (RuntimeException)
    {
        CEvent::preventDefault();
    }

    void SAL_CALL CUIEvent::initEvent(const OUString& eventTypeArg, sal_Bool canBubbleArg,
        sal_Bool cancelableArg) throw (RuntimeException)
    {
        // base initializer
        CEvent::initEvent(eventTypeArg, canBubbleArg, cancelableArg);
    }
}}
