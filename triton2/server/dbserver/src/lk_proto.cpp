#include "lk_proto.h"
#include "base.h"

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
void print_fieldvalue(const Message& message, const Reflection* reflection, 
		        const FieldDescriptor* field, int index, CMaxString& generator, ARRAY_FORMAT array_format,
				        void (*pformat)(CMaxString&, const char*, const char*, const char*))
{
	if ( !field->is_repeated() && index != -1) 
	{
		printf("Index must be -1 for non-repeated fields:%s\n", field->name().c_str());
		return;
	}

	switch (field->cpp_type()) 
	{
		#define OUTPUT_FIELD(CPPTYPE, METHOD, TO_STRING)    \
		case FieldDescriptor::CPPTYPE_##CPPTYPE:            \
		{                                                   \
			pformat(generator, \
				message.GetDescriptor()->name().c_str(), \
				field->name().c_str(), \
				TO_STRING ( field->is_repeated() ?  \
				reflection->GetRepeated##METHOD(message, field, index): \
				reflection->Get##METHOD(message, field)).c_str());      \
				break;  \
		}				\


		OUTPUT_FIELD( INT32,  Int32, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD( INT64,  Int64, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD(UINT32, UInt32, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD(UINT64, UInt64, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD( FLOAT,  Float, ::google::protobuf::SimpleFtoa);
		OUTPUT_FIELD(DOUBLE, Double, ::google::protobuf::SimpleDtoa);
		#undef OUTPUT_FIELD

		case FieldDescriptor::CPPTYPE_STRING: 
		{
			std::string scratch;

			const std::string& value = field->is_repeated() ?
				reflection->GetRepeatedStringReference(
						message, field, index, &scratch) :
				reflection->GetStringReference(message, field, &scratch);

			pformat(generator,
					message.GetDescriptor()->name().c_str(),
					field->name().c_str(), value.c_str());
			break;
		}

		case FieldDescriptor::CPPTYPE_BOOL:
		{
			if (field->is_repeated())
			{
				pformat(generator,
						message.GetDescriptor()->name().c_str(),
						field->name().c_str(),
						reflection->GetRepeatedBool(message, field, index) ? "true" : "false");
			} else
			{
				pformat(generator,
						message.GetDescriptor()->name().c_str(),
						field->name().c_str(),
						reflection->GetBool(message, field) ? "true" : "false");
			}
			break;
		}

		case FieldDescriptor::CPPTYPE_ENUM:
		{
			pformat(generator,
					message.GetDescriptor()->name().c_str(),
					field->name().c_str(),
					field->is_repeated() ?
					reflection->GetRepeatedEnum(message, field, index)->name().c_str() :
					reflection->GetEnum(message, field)->name().c_str());
			break;
		}

		case FieldDescriptor::CPPTYPE_MESSAGE:
		{
			const Message & submsg = (field->is_repeated() 
									?  reflection->GetRepeatedMessage(message, field, index) 
									: reflection->GetMessage(message, field) );

			if( array_format != NULL )
			{
//			generator.append("<%s proto=\"%s\">\n", field->name().c_str(), submsg.GetDescriptor()->name().c_str());
				array_format(submsg, generator, field->name().c_str(), submsg.GetDescriptor()->name().c_str( ), 
					pformat);
//			generator.append("</%s>\n", field->name().c_str());
			}
			else
			{
				char szFilterBuff[ 1024 * 200 ] = { 0 };
				char cTempItemBuff[ 1024 * 200 ] = { 0 };

				char* pszTmpStr = szFilterBuff;

				if( submsg.SerializeToArray( cTempItemBuff, sizeof(cTempItemBuff) -1 ) == true )
				{
					int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szFilterBuff)-1, cTempItemBuff, 
						submsg.ByteSize() );
					if( tRealLen != -1 )
					{
						pformat( generator, submsg.GetDescriptor()->name().c_str( ), field->name().c_str(),
						 pszTmpStr );	
					}
				}
				
			}

			break;
		}
	}
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
void xml_format(CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value)
{
	generator.append("<%s>%s</%s>\n",  field_name, field_value, field_name);
}


// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
void print_field(const Message& message,
		const Reflection* reflection, const FieldDescriptor* field, CMaxString& generator, ARRAY_FORMAT array_format,
		void (*pformat)(CMaxString&, const char*, const char*, const char*))
{
	int count = 0;

	if (field->is_repeated())
	{
		count = reflection->FieldSize(message, field);
	} else if (reflection->HasField(message, field))
	{
		count = 1;
	}

	for (int j = 0; j < count; ++j)
	{
		int field_index = field->is_repeated() ? j : -1;
		print_fieldvalue(message, reflection, field, field_index, generator, array_format, pformat);
	}
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
void protobuf2xml(const Message& message, CMaxString& debug_string, const char* root_name, const char* root_type,
		void (*pformat)(CMaxString&, const char*, const char*, const char*))
{
	const Reflection* reflection = message.GetReflection();
	std::vector<const FieldDescriptor*> fields;
	reflection->ListFields(message, &fields);

	debug_string.append( "<%s proto=\"%s\">\n", root_name, root_type );
	for (unsigned int i = 0; i < fields.size(); i++)
	{
		print_field(message, reflection, fields[i], debug_string, protobuf2xml, pformat);
	}
	debug_string.append( "</%s>\n", root_name );
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************

int xml2protobuf(const TiXmlNode *parent, Message* pMessage, const FieldDescriptor* pFieldDef)
{
	if ( parent == NULL || pMessage == NULL ) return -1;

	const Reflection* pReflection = pMessage->GetReflection();
	const Descriptor* pDescriptor = pMessage->GetDescriptor();

	//const FieldDescriptor *pFieldDef = NULL; 

	//遍历xml, 防止取xml节点不完整
	const TiXmlNode*  child = parent->FirstChild();
	for (; child != NULL; child = child->NextSibling())
	{
		switch(child->Type())
		{
			case TiXmlNode::ELEMENT:
			{
				pFieldDef = pDescriptor->FindFieldByName(child->Value());
				if (pFieldDef == NULL)
				{
					printf("Message (%s) has no field (%s), pls check protocol\n",
							pDescriptor->name().c_str(), child->Value()); 
					return -1;
				}

				Message* pSubMessage = NULL;
				if ( pFieldDef->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE)
				{
					pSubMessage = pFieldDef->is_repeated() ?
									pReflection->AddMessage(pMessage, pFieldDef):
									pReflection->MutableMessage(pMessage, pFieldDef);

					if ( pSubMessage == NULL )
					{
						printf("Can not add message for field (%s) of message (%s)\n", 
								child->Value(), pDescriptor->name().c_str()); 
						return -1;
					}
				}

				if (xml2protobuf( child, pSubMessage ? pSubMessage : pMessage, pFieldDef) != 0)
				{
					return -1;
				}

				break;
			}

			case TiXmlNode::TEXT:
			{
				bool is_repeated = pFieldDef->is_repeated();
				switch(pFieldDef->cpp_type())
				{
					case FieldDescriptor::CPPTYPE_INT32:
					{
						is_repeated 
							? pReflection->AddInt32(pMessage, pFieldDef, atoi(child->Value()))
							: pReflection->SetInt32(pMessage, pFieldDef, atoi(child->Value()));

						break;
					}

					case FieldDescriptor::CPPTYPE_UINT32:
					{
						is_repeated 
							? pReflection->AddUInt32(pMessage, pFieldDef, strtoul(child->Value(), NULL, 0))
							: pReflection->SetUInt32(pMessage, pFieldDef, strtoul(child->Value(), NULL, 0));
						break;
					}

					case FieldDescriptor::CPPTYPE_INT64:
					{
						is_repeated 
							? pReflection->AddInt64(pMessage, pFieldDef, strtoll(child->Value(), NULL, 0))
							: pReflection->SetInt64(pMessage, pFieldDef, strtoll(child->Value(), NULL, 0));

						break;
					}

					case FieldDescriptor::CPPTYPE_UINT64:
					{
						is_repeated 
							? pReflection->AddUInt64(pMessage, pFieldDef, strtoull(child->Value(), NULL, 0))
							: pReflection->SetUInt64(pMessage, pFieldDef, strtoull(child->Value(), NULL, 0));
						break;
					}

					case FieldDescriptor::CPPTYPE_STRING:
					{
						is_repeated 
							? pReflection->AddString(pMessage, pFieldDef, child->Value())
							: pReflection->SetString(pMessage, pFieldDef, child->Value());

						break;
					}

					case FieldDescriptor::CPPTYPE_BOOL:
					{
						bool is_true = strcasecmp("true", child->Value());

						is_repeated 
							? pReflection->AddBool(pMessage, pFieldDef, is_true)
							: pReflection->SetBool(pMessage, pFieldDef, is_true);

						break;
					}

					case FieldDescriptor::CPPTYPE_ENUM:
					{
						const EnumValueDescriptor* pEnumValue = pDescriptor->FindEnumValueByName(child->Value());
						if ( pEnumValue == NULL)
						{
							printf("Can not fine enum text(%s) of message(%s)\n", 
									child->Value(), pFieldDef->name().c_str(), pDescriptor->name().c_str()); 

							return -1;
						}

						is_repeated 
							? pReflection->AddEnum(pMessage, pFieldDef, pEnumValue)
							: pReflection->SetEnum(pMessage, pFieldDef, pEnumValue);

						break;
					}

					default:
					{
						printf("The Msg(%s) don't support field(%s) type(%d) value(%s)", 
								pDescriptor->name().c_str(), pFieldDef->name().c_str(), pFieldDef->type(), child->Value());
						return -1;
					}
				}

				break;
			}

			default:
			{
				break;
			}
		}
	}

	return 0;
}


// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           12/30/2009
// 
// ***************************************************************

void protobuf2Sql( const Message& message, CMaxString& debug_string, const char* root_name, const char* root_type, 
	OUT_FORMAT out_format )
{
	const Reflection* reflection = message.GetReflection();
	std::vector<const FieldDescriptor*> fields;
	reflection->ListFields(message, &fields);

	for (unsigned int i = 0; i < fields.size(); i++)
	{
		if( i != 0 )
		{
			debug_string.append( "," );
		}
		print_field(message, reflection, fields[i], debug_string, NULL, out_format );
	}
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           12/30/2009
// 
// ***************************************************************

void sql_format( CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value )
{
	generator.append( " %s=\'%s\' ", field_name, field_value );
}


