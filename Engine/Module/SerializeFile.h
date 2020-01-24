#pragma once

//*********************************************************************************************************************
//
//	Serialize
//
//*********************************************************************************************************************
namespace Serialize
{
	//型名配列
	extern std::vector<std::string> TypeArray;
}

#define SERIALIZE_TYPE(type) Serialize::TypeArray.push_back(#type);

//*********************************************************************************************************************
//
//	InputValue
//		valueをType型に変換する。特殊化していない場合、assertで実行停止
//
//*********************************************************************************************************************

template<typename Type>
void InputValue(Type& type,const char* value)
{
	assert(0);
}

#ifndef SERIALIZE_H

template<>
void InputValue(std::string& type, const char* value)
{
	type = value;
}

template<>
void InputValue(int& type, const char* value)
{
	type = atoi(value);
}

template<>
void InputValue(float& type, const char* value)
{
	type = atof(value);
}

template<>
void InputValue(bool& type, const char* value)
{
	type = (atoi(value) == 1);
}

#endif

//*********************************************************************************************************************
//
//	LoadData
//		data文字列から「,」区切りでデータを変換する
//
//*********************************************************************************************************************

//末尾処理
inline void LoadData(std::string data)
{
	return;
}

template<typename First,typename... Rect>
void LoadData(std::string data,First& first,Rect&... rect)
{
	size_t count;
	count = data.find_first_of(",");
	InputValue(first, data.substr(0, count).data());
	LoadData(data.substr(count + 1), rect...);
}

//データ文字列読み込み
template<typename Type,typename First,typename... Rect>
void LoadData(FILE* file,First& first,Rect&... rect)
{
	char line[256];
	fgets(line,256,file);
	LoadData(line,first,rect...);
}


//*********************************************************************************************************************
//
//	OutputValue
//
//*********************************************************************************************************************

//#define TO_STRING(VariableName) # VariableName
//#define OUTPUT_VALUE(Value) OutputValue(#Value,Value)

template<typename Type>
std::string OutputValue(Type value)
{
	return std::to_string(value);
}


#ifndef SERIALIZE_H

template<>
std::string OutputValue(std::string value)
{
	return value;
}

template<typename First,typename... Rect>
std::string OutputValue(First& first,Rect&... rect)
{
	return OutputValue(first) +","+  OutputValue(rect...);
}

#endif

//*********************************************************************************************************************
//
//	SaveData
//
//*********************************************************************************************************************
template<typename Type,typename First,typename... Rect>
void SaveData(FILE* file,First& first,Rect&... rect)
{
	std::string data;
	data += typeid(Type).name();
	data += "\n";
	data += OutputValue(first,rect...);

	fwrite(data.data(),data.size(),1,file);
	fseek(file,0,SEEK_END);
}



#ifndef SERIALIZE_H
#define SERIALIZE_H
#endif // !SERIALIZE_H
