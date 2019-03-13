//
//  contacts.h
//  hyc
//
//  Created by brainpoint on 12-1-7.
//  Copyright (c) 2012骞?hy. All rights reserved.
//

#ifndef HYC_MOBILE_CONTACTS_H
#define HYC_MOBILE_CONTACTS_H
#ifdef HYC_COMPILE_MODULE_MOBILE


namespace hymobile {
    
    //----------------------------------------------------------------------------------------------------
    //!	Contains properties that describe a contact, such as a user's personal or business contact.
    //----------------------------------------------------------------------------------------------------
    class HYC_API Contact : public RefCount
    {
    public:
        //! Contact name
        typedef struct {
            String  _givenName;
            String  _middleName;
            String  _familyName;
        } ContactName;
        
    public:
        //! save or remove this contact
        bool            save();
        bool            remove();
        
        //! attribute
        const String&   id() const;     // A globally unique identifier. (DOMString)
        String&         displayName();  // The name of this Contact, suitable for display to end-users. (DOMString)
        ContactName&    name();         // An object containing all components of a persons name. (ContactName)
    };
    typedef ObjectPtr<Contact>  ContactPtr;
    
    
    //----------------------------------------------------------------------------------------------------
    //!	The contacts object provides access to the device contacts database.
    //----------------------------------------------------------------------------------------------------
    class HYC_API Contacts
    {
    public:
        /**
         * @brief Create a new contact. 
         */
        static ContactPtr   Create( const String& displayName );
        
        /**
         * @brief Find the contact, if findStr == null then will find out all contact
         */
        static void         Find( const String& findStr, OUT Array<ContactPtr>* arrContact );
    };
    
} // end namespace of hymobile

#endif
#endif // HYC_MOBILE_CONTACTS_H
