#pragma once
/*
ProtocolIdentifier: 0x54454E2E
MajorVersion: 1 (0x1)
MinorVersion: 0 (0x0)
Operation: Request (0x00)
Content Length
   Content Distribution: Content Length (0x00)
   Content Length: 1013 (0x03F5)
Header 1:
   RequestUriHeader
      HeaderToken: RequestUri (0x04)
      DataType:  CountedString (0x01)
      StringEncoding: UTF8 (0x01)
      UriValue: tcp://maheshdev2:8080/RemoteActivationService.rem
Header 2:
   ContentTypeHeader: 
      HeaderToken: ContentType (0x06)
      DataType: CountedString (0x01)
      ContentTypeValue: application/octet-stream
Header 3:
   EndHeader: 
      HeaderToken: EndOfHeaders(0x00)

Binary Serialization Format
   SerializationHeaderRecord:
      BinaryHeaderEnum: SerializedStreamHeader (0x00)
      TopId: 1 (0x1)
      HeaderId: -1 (0xFFFFFFFF)
      MajorVersion: 1 (0x1)
      MinorVersion: 0 (0x0)
   BinaryMethodCall: 
      BinaryHeaderEnum: BinaryMethodCall (0x15)
      MessageEnum: 00000012
         NoArgs:                 (...............................0)
         ArgsInline:             (..............................0.)
         ArgsIsArray:            (.............................1..)
         ArgsInArray:            (............................0...)
         NoContext:              (...........................1....)
         ContextInline:          (..........................0.....)
         ContextInArray:         (.........................0......)
         MethodSignatureInArray: (........................0.......)
         PropertyInArray:        (.......................0........)
         NoReturnValue:          (......................0.........)
         ReturnValueVoid:        (.....................0..........)
         ReturnValueInline:      (....................0...........)
         ReturnValueInArray:     (...................0............)
         ExceptionInArray:       (..................0.............)
         Reserved:               (000000000000000000..............)
      MethodName: 
         PrimitiveTypeEnum: String (0x12)
         Data: Activate
      TypeName: 
         PrimitiveTypeEnum: String (0x12)
         Data: System.Runtime.Remoting.Activation.IActivator, 
               mscorlib, Version=2.0.0.0, Culture=neutral, 
               PublicKeyToken=b77a5c561934e089
      ArgsCount: 0 (0x0)

   CallArray:   
      ArraySingleObject:
         ObjectId: 1 (0x1)
         Length: 1 (0x1)
      MemberReference:
         IdRef: 2
         SystemClassWithMembersAndTypes:
         BinaryHeaderEnum: SystemClassWithMembersAndTypes (0x04)
         ObjectId: 2
         Name: System.Runtime.Remoting.Messaging.ConstructionCall
         NumMembers: 11 (0x0B)
            MemberNames:
               Data: __Uri
            MemberNames: 
               Data: __MethodName
            MemberNames:
               Data: __MethodSignature
            MemberNames:
               Data: __TypeName
            MemberNames:
               Data: __Args
            MemberNames:
               Data: __CallContext
            MemberNames:
               Data: __CallSiteActivationAttributes
            MemberNames:
               Data: __ActivationType
            MemberNames:
               Data: __ContextProperties
            MemberNames:
               Data: __Activator
            MemberNames:
               Data __ActivationTypeName
      BinaryTypeEnumA:
         Object  (0x02)
         String  (0x01)
         SystemClass (0x03)
         String  (0x01)
         ObjectArray (0x05)
         Object (0x02)
         Object (0x02)
         Object (0x02)
         SystemClass (0x03)
         SystemClass (0x03)
         String  (0x01)
      AdditionalTypeInformationArray:
         SystemClass:
            Length: 13 (0x0D)
            Data: System.Type[]
         SystemClass:
            Length: 28 (0x1C)
            Data: System.Collections.ArrayList
         SystemClass:
            Length: 56 (0x38)
            Data: System.Runtime.Remoting.Activation.
                  ContextLevelActivator
   ObjectNull:
      BinaryHeaderEnum: ObjectNull (0x0A)
   BinaryObjectString:
      BinaryHeaderEnum: BinaryObjectString (0x06)
      ObjectId: 3 (0x03)
      Length: 5 (0x05)
      Value: .ctor
   MemberReference:
      BinaryHeaderEnum: MemberReference (0x09)
      IdRef: 4 (0x04)
   BinaryObjectString:
      BinaryHeaderEnum: BinaryObjectString (0x06)
      ObjectId: 5 (0x05)
      Length: 111 (0x06F)
      Value: DOJRemotingMetadata.MyServer, DOJRemotingMetadata, 
             Version=1.0.2616.21414, Culture=neutral, 
             PublicKeyToken=null
   MemberReference:
      BinaryHeaderEnum: MemberReference (0x09)
      IdRef: 6 (0x06)
   ObjectNull:
      BinaryHeaderEnum: ObjectNull (0x0A)
   ObjectNull:
      BinaryHeaderEnum: ObjectNull (0x0A)
   ObjectNull:
      BinaryHeaderEnum: ObjectNull (0x0A)
   MemberReference:
      BinaryHeaderEnum: MemberReference (0x09)
      IdRef: 7 (0x07)
   MemberReference:
      BinaryHeaderEnum: MemberReference (0x09)
      IdRef: 8 (0x08)
   MemberReference:
      BinaryHeaderEnum: MemberReference (0x09)
      IdRef: 5 (0x05)
   BinaryArray:
      BinaryHeaderEnum: BinaryArray (0x07)
      ObjectId: 4 (0x04)
      BinaryArrayTypeEnum: 0 (0x0)
      Rank: 1 (0x1)
      LengthA: 0 (0x0)
      BinaryTypeEnumA:
         ObjectUrt (0x03)
            Length: 11 (0x0B)
            Value: System.Type
   SystemClassWithMembersAndTypes:
      BinaryHeaderEnum: SystemClassWithMembersAndTypes (0x04)
      ObjectId: 7 (0x07)
      Name: System.Collections.ArrayList
      NumMembers: 3 (0x03)
         MemberNames:
            Data: _items
         MemberNames: 
            Data: _size
         MemberNames:
            Data: _version
      BinaryTypeEnumA:
         ObjectArray (0x05)
         Primitive  (0x00)
         Primitive  (0x00)
      Additional Type Information:
         Primitive Type: Int32 (0x08)
         Primitive Type: Int32 (0x08)
   Object Information Array:
      MemberReference:
         BinaryHeaderEnum: MemberReference (0x09)
         IdRef: 10 (0x0A)
      MemberPrimitiveUnTyped:
         Value: 0 (0x00)
      MemberPrimitiveUnTyped:
         Value: 0 (0x00)
   SystemClassWithMembersAndTypes:
      BinaryHeaderEnum: SystemClassWithMembersAndTypes (0x04)
      ObjectId: 8
      Name: System.Runtime.Remoting.Activation.ContextLevelActivator
      NumMembers: 1 (0x01)
         MemberNames:
            Data: m_NextActivator
      BinaryTypeEnumA:
         ObjectUrt (0x03)
      Additional Type Information:
         ObjectUrt:
            Length: 61 (0x3D)
            Data: System.Runtime.Remoting.Activation.
                  ConstructionLevelActivator
   MemberReference:
      BinaryHeaderEnum: MemberReference (0x09)
      IdRef: 11 (0x0B)
   ArraySingleObject:
      BinaryHeaderEnum: ArraySingleObject (0x10)
      ObjectId: 10 (0x0A)
      Length: 0 (0x00)
   BinaryObjectWithMapTyped:
      BinaryHeaderEnum: SystemClassWithMembersAndTypes (0x04)
      ObjectId: 11
      Name: System.Runtime.Remoting.Activation.
            ConstructionLevelActivator
      NumMembers: 0 (0x00)
   MessageEnd:
      BinaryHeaderEnum: MessageEnd (0x0B)
	  
From: http://msdn.microsoft.com/en-us/library/cc236994.aspx
	  */

//Like BinaryHeaderEnum
enum ESchemaType
{
	eSchemaType_Invalid						=   -1,
	eSchemaType_SerializedStreamHeader		= 0x00,
	eSchemaType_ArrayItem					= 0x01,
	eSchemaType_SystemClass					= 0x04,
	eSchemaType_Class						= 0x05,
	eSchemaType_BinaryObjectString			= 0x06,
	eSchemaType_BinaryArray					= 0x07,
	eSchemaType_BoxedPrimitiveType			= 0x08,
	eSchemaType_MemberReference				= 0x09,
	eSchemaType_ObjectNull					= 0x0A,
	eSchemaType_End							= 0x0B,
	eSchemaType_Assembly					= 0x0C,
	eSchemaType_ArrayFiller8b				= 0x0D,
	eSchemaType_ArrayFiller32b				= 0x0E,
	eSchemaType_ArrayOfPrimitiveType		= 0x0F,
	eSchemaType_ArrayOfObject				= 0x10,
	eSchemaType_ArrayOfString				= 0x11,
	eSchemaType_BinaryMethodCall			= 0x15,
	eSchemaType_BinaryMethodReturn			= 0x16,
};