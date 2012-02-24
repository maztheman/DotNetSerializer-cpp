// Binary.Serializer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BinaryStream.h"
#include "DotNetFileReader.h"

#include <fstream>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc != 2 )
	{
		cout << "Usage: DotNetSerializer.exe <file>" << endl;
		return 0;
	}

	fstream fs( argv[1], ios_base::in | ios_base::binary );
	if ( !fs.is_open() )
	{
		cout << "File doesnt exist" << endl;
		return 0;
	}
	CBinaryStream stream;
	stream.InitializeFromStream( fs );
	fs.close();
	CDotNetFileReader reader( stream );
	CDotNetClass* pObject = reader.Deserialize();

	if ( pObject )
	{
		string sHello = pObject->GetString("StringValue");
		delete pObject;
	}

	return 0;
}

