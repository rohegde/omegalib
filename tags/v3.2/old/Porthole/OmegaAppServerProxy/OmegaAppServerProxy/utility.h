//
//  utility.h
//  OmegaViewerServerProxy
//
//  Created by Dennis Chau on 10/10/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#ifndef utility_h
#define utility_h

#include <string>

using namespace std;

string unsignedLongToString( unsigned long length );
string intToString( int theVal );
string floatToString( float theVal );
const char * stringToCStr ( string inputString );


string unsignedLongToString( unsigned long theVal )
{
    char buffer [128];
    sprintf( buffer , "%lu" , theVal );
    return std::string(buffer);
    
}

string intToString( int theVal )
{
    char buffer [128];
    sprintf( buffer , "%d" , theVal );
    return std::string(buffer);
    
}

string floatToString( float theVal )
{
    char buffer [128];
    sprintf( buffer , "%.2f" , theVal );
    return std::string(buffer);
    
}



#endif
