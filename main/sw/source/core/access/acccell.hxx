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


#ifndef _ACCCELL_HXX
#define _ACCCELL_HXX

#include "acccontext.hxx"
#include <com/sun/star/accessibility/XAccessibleValue.hpp>

#ifndef _COM_SUN_STAR_ACCESSIBILITY_XAccessibleExtendedAttributes_HPP_
#include <com/sun/star/accessibility/XAccessibleExtendedAttributes.hpp>
#endif

#ifndef _ACCSELECTIONHELPER_HXX_
#include <accselectionhelper.hxx>
#endif

class SwCellFrm;
class SwAccessibleTable;
class SwFrmFmt;

class SwAccessibleCell : public	SwAccessibleContext,
                  ::com::sun::star::accessibility::XAccessibleValue,
                  ::com::sun::star::accessibility::XAccessibleSelection, 
					public  ::com::sun::star::accessibility::XAccessibleExtendedAttributes
{
    // Implementation for XAccessibleSelection interface
    SwAccessibleSelectionHelper aSelectionHelper;
	sal_Bool	bIsSelected;	// protected by base class mutex

    ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > m_xTableReference;
    SwAccessibleTable *m_pAccTable;

    sal_Bool	IsSelected();

	sal_Bool _InvalidateMyCursorPos();
	sal_Bool _InvalidateChildrenCursorPos( const SwFrm *pFrm );

protected:

	// Set states for getAccessibleStateSet.
	// This drived class additionally sets SELECTABLE(1) and SELECTED(+)
	virtual void GetStates( ::utl::AccessibleStateSetHelper& rStateSet );

	virtual void _InvalidateCursorPos();

	virtual ~SwAccessibleCell();

public:

    SwAccessibleCell( SwAccessibleMap* pInitMap, const SwCellFrm *pCellFrm );

	virtual sal_Bool HasCursor();	// required by map to remember that object

	//=====  XAccessibleContext  ==============================================

    ///	Return this object's description.
	virtual ::rtl::OUString SAL_CALL
    	getAccessibleDescription (void)
        throw (com::sun::star::uno::RuntimeException);

	//=====  XServiceInfo  ====================================================

    /**	Returns an identifier for the implementation of this object.
    */
	virtual ::rtl::OUString SAL_CALL
    	getImplementationName (void)
        throw (::com::sun::star::uno::RuntimeException);

    /**	Return whether the specified service is supported by this class.
    */
    virtual sal_Bool SAL_CALL
    	supportsService (const ::rtl::OUString& sServiceName)
        throw (::com::sun::star::uno::RuntimeException);

    /** Returns a list of all supported services.  In this case that is just
    	the AccessibleContext service.
    */
	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString> SAL_CALL
    	getSupportedServiceNames (void)
        throw (::com::sun::star::uno::RuntimeException);

	virtual void Dispose( sal_Bool bRecursive = sal_False );

	virtual void InvalidatePosOrSize( const SwRect& rFrm );

	//=====  XInterface  ======================================================

    // (XInterface methods need to be implemented to disambiguate
    // between those inherited through SwAcessibleContext and
    // XAccessibleValue).

    virtual ::com::sun::star::uno::Any SAL_CALL queryInterface(
        const ::com::sun::star::uno::Type& aType )
        throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL acquire(  ) throw ()
        { SwAccessibleContext::acquire(); };

    virtual void SAL_CALL release(  ) throw ()
        { SwAccessibleContext::release(); };

	//====== XTypeProvider ====================================================
	virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes(  ) throw(::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId(  ) throw(::com::sun::star::uno::RuntimeException);

	//=====  XAccessibleValue  ================================================

	//=====  XAccessibleExtendedAttributes ================================================	
	::com::sun::star::uno::Any SAL_CALL getExtendedAttributes() 
		throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException) ;
private:
    SwFrmFmt* GetTblBoxFormat() const;

public:
    virtual ::com::sun::star::uno::Any SAL_CALL getCurrentValue( )
        throw (::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL setCurrentValue(
        const ::com::sun::star::uno::Any& aNumber )
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getMaximumValue(  )
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getMinimumValue(  )
        throw (::com::sun::star::uno::RuntimeException);
	//=====  XAccessibleComponent  ============================================
	sal_Int32 SAL_CALL getBackground() 
		throw (::com::sun::star::uno::RuntimeException);

	//=====  XAccessibleSelection  ============================================
	virtual void SAL_CALL selectAccessibleChild( 
        sal_Int32 nChildIndex ) 
        throw ( ::com::sun::star::lang::IndexOutOfBoundsException, 
                ::com::sun::star::uno::RuntimeException );

    virtual sal_Bool SAL_CALL isAccessibleChildSelected( 
        sal_Int32 nChildIndex ) 
        throw ( ::com::sun::star::lang::IndexOutOfBoundsException, 
                ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL clearAccessibleSelection(  ) 
        throw ( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL selectAllAccessibleChildren(  ) 
        throw ( ::com::sun::star::uno::RuntimeException );
    virtual sal_Int32 SAL_CALL getSelectedAccessibleChildCount(  ) 
        throw ( ::com::sun::star::uno::RuntimeException );
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getSelectedAccessibleChild( 
        sal_Int32 nSelectedChildIndex ) 
        throw ( ::com::sun::star::lang::IndexOutOfBoundsException, 
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL deselectAccessibleChild( 
        sal_Int32 nSelectedChildIndex ) 
        throw ( ::com::sun::star::lang::IndexOutOfBoundsException, 
                ::com::sun::star::uno::RuntimeException );

};


#endif

