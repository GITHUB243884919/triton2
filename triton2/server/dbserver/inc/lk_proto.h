#ifndef _DB_PROTO_
#define _DB_PROTO_
#include "google/protobuf/text_format.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/unknown_field_set.h"
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/stubs/strutil.h"

#include "lk_string.h"
#include "tinyxml.h"

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;


typedef lk::string<1024000> CMaxString;
typedef void (*OUT_FORMAT)( CMaxString&, const char*, const char*, const char*);


typedef void (*ARRAY_FORMAT)( const Message&, CMaxString&, const char*, const char*, OUT_FORMAT ) ;



void print_fieldvalue(const Message& message, const Reflection* reflection, 
		const FieldDescriptor* field, int index, CMaxString& generator, ARRAY_FORMAT,
		void (*pformat)(CMaxString&, const char*, const char*, const char*));

void print_field(const Message& message,
		const Reflection* reflection, const FieldDescriptor* field, CMaxString& generator, 	ARRAY_FORMAT array_format,
		void (*pformat)(CMaxString&, const char*, const char*, const char*));

void protobuf2xml(const Message& message, CMaxString& debug_string, const char* root_name, const char* root_type,
		void (*pformat)(CMaxString&, const char*, const char*, const char*));

int xml2protobuf(const TiXmlNode *parent, Message* pMessage, const FieldDescriptor* pFieldDef);

void xml_format(CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value);

void protobuf2Sql( const Message& message, CMaxString& debug_string, const char* root_name, const char* root_type, 
	OUT_FORMAT out_format );

void sql_format( CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value );



#endif


