//
//  connection.h
//  hyc
//
//  Created by brainpoint on 12-1-7.
//  Copyright (c) 2012骞?hy. All rights reserved.
//

#ifndef HYC_MOBILE_CONNECTION_H
#define HYC_MOBILE_CONNECTION_H

namespace hymobile {
    
    //! connection type
    typedef enum {
        ConnectionType_NONE     = 0,
        ConnectionType_ETHERNET,
        ConnectionType_WIFI,
        ConnectionType_CELL_2G,
        ConnectionType_CELL_3G,
        ConnectionType_CELL_4G,
    } ConnectionType;
}


#ifdef HYC_COMPILE_MODULE_MOBILE
namespace hymobile {
    
    /**
     * connection status change delegate.
     */
    class HYC_API ConnectionDelegate
    {
    public:
        ConnectionDelegate();
        virtual ~ConnectionDelegate();
        
        virtual void    onConnectionTypeChange( ConnectionType oldType, ConnectionType newType ) = 0;
    };
    
    
    /**
     * The connection object gives access to the device's cellular and wifi connection information.
     */
    class HYC_API Connection
    {
    public:
        /**
         * @brief Get the device current connection status.
         */
        static ConnectionType   GetConnectionType();
    };
    
    
} // end namespace of hymobile

#endif
#endif // HYC_MOBILE_CONNECTION_H
