//
// Created by bytedance on 2020/7/28.
//

#include "CompilationInfo.h"
#include "Colors.h"
#include "external/rapidjson/document.h"
#include "external/rapidjson/writer.h"
#include "external/rapidjson/stringbuffer.h"
#include <chrono>
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <iostream>
#include <sys/stat.h>
#include <fstream>

using namespace rapidjson;
CompilationInfo::CompilationInfo(FILE *out, WholeProcessInfo mWholeProcessInfo)
        : out(out), m_wholeProcessInfo(mWholeProcessInfo) {
    m_ParseFilesInfos.reserve(50);
}
CompilationInfo::~CompilationInfo() = default;

void CompilationInfo::GenerateJsonFile() {
    fprintf(out, "%s%s**** Start generating json files%s:\n", col::kBold, col::kMagenta, col::kReset);
    if (mkdir("jsons", 0777) == -1)
    {
        std::cerr << "Error :  jsons directory exists" <<std::endl;
    }
    else
    {
        std::cout << "Directory created\n";
    }
    GenerateParseFilesJsonFile();
    GenerateCodegenFilesJsonFile();
    GenerateTemplatesJsonFile();
    GenerateTemplateSetsJsonFile();
    GenerateFunctionInfosJsonFile();
    GenerateFunctionSetInfosJsonFile();
    GenerateExpensiveHeaderInfosJsonFile();
    GenerateWholeProcessInfo();
    fprintf(out, "%s%s**** End generating json files%s:\n", col::kBold, col::kMagenta, col::kReset);
}

void CompilationInfo::GenerateParseFilesJsonFile()
{
    fprintf(out, "***********Generate Parse Files JsonFile (compiler frontend)*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Parse Files");

    writer.Key("File Names");
    writer.StartArray();
    for(const auto & info :m_ParseFilesInfos)
    {
        writer.Key(info.name.data());
    }
    writer.EndArray();
    writer.Key("Time Spend");
    writer.StartArray();
    for(const auto & info :m_ParseFilesInfos)
    {
        writer.Int(info.time_ms);
    }
    writer.EndArray();
    writer.EndObject();
    std::ofstream outFile( "jsons/ParseFiles.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateCodegenFilesJsonFile()
{
    fprintf(out, "***********Generate Codegen Files Json File (compiler backend)*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Codegen Files");

    writer.Key("File Names");
    writer.StartArray();
    for(const auto & info :m_CodegenFilesInfos)
    {
        writer.Key(info.name.data());
    }
    writer.EndArray();
    writer.Key("Time Spend");
    writer.StartArray();
    for(const auto & info :m_CodegenFilesInfos)
    {
        writer.Int(info.time_ms);
    }
    writer.EndArray();
    writer.EndObject();
    std::ofstream outFile( "jsons/CodegenFiles.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateTemplatesJsonFile()
{
    fprintf(out, "***********Generate Templates Instantiations Json File*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Templates Instantiations");

    writer.Key("File Names");
    writer.StartArray();
    for(const auto & info :m_TemplatesInfos)
    {
        writer.Key(info.name.data());
    }
    writer.EndArray();

    writer.Key("Time Spend");
    writer.StartArray();
    for(const auto & info :m_TemplatesInfos)
    {
        writer.Int(info.time_ms);
    }
    writer.EndArray();

    writer.Key("Instantiation times");
    writer.StartArray();
    for(const auto & info :m_TemplatesInfos)
    {
        writer.Int(info.ins_times);
    }
    writer.EndArray();

    writer.Key("Average time");
    writer.StartArray();
    for(const auto & info :m_TemplatesInfos)
    {
        writer.Int(info.avg_time);
    }
    writer.EndArray();

    writer.EndObject();
    std::ofstream outFile( "jsons/Instantiations.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateTemplateSetsJsonFile()
{
    fprintf(out, "***********Generate Templates Sets Instantiations JsonFile*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Templates Sets Instantiations");

    writer.Key("File Names");
    writer.StartArray();
    for(const auto & info :m_TemplateSetsInfos)
    {
        writer.Key(info.name.data());
    }
    writer.EndArray();

    writer.Key("Time Spend");
    writer.StartArray();
    for(const auto & info :m_TemplateSetsInfos)
    {
        writer.Int(info.time_ms);
    }
    writer.EndArray();

    writer.Key("Instantiation times");
    writer.StartArray();
    for(const auto & info :m_TemplateSetsInfos)
    {
        writer.Int(info.ins_times);
    }
    writer.EndArray();

    writer.Key("Average time");
    writer.StartArray();
    for(const auto & info :m_TemplateSetsInfos)
    {
        writer.Int(info.avg_time);
    }
    writer.EndArray();

    writer.EndObject();
    std::ofstream outFile( "jsons/InstantiationsSets.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateFunctionInfosJsonFile()
{
    fprintf(out, "***********Generate Functions Json File*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Functions");

    writer.Key("Function Names");
    writer.StartArray();
    for(const auto & info :m_FunctionInfos)
    {
        writer.Key(info.func_name.data());
    }
    writer.EndArray();

    writer.Key("Time Spend");
    writer.StartArray();
    for(const auto & info :m_FunctionInfos)
    {
        writer.Int(info.time_ms);
    }
    writer.EndArray();

    writer.Key("File Names");
    writer.StartArray();
    for(const auto & info :m_FunctionInfos)
    {
        writer.String(info.file_name.data());
    }
    writer.EndArray();

    writer.EndObject();
    std::ofstream outFile( "jsons/Functions.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateFunctionSetInfosJsonFile()
{
    fprintf(out, "***********Generate Function Sets Json File*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Function Sets");

    writer.Key("File Names");
    writer.StartArray();
    for(const auto & info :m_FunctionSetInfos)
    {
        writer.Key(info.name.data());
    }
    writer.EndArray();

    writer.Key("Time Spend");
    writer.StartArray();
    for(const auto & info :m_FunctionSetInfos)
    {
        writer.Int(info.time_ms);
    }
    writer.EndArray();

    writer.Key("Compile/optimization times");
    writer.StartArray();
    for(const auto & info :m_FunctionSetInfos)
    {
        writer.Int(info.ins_times);
    }
    writer.EndArray();

    writer.Key("Average time");
    writer.StartArray();
    for(const auto & info :m_FunctionSetInfos)
    {
        writer.Int(info.avg_time);
    }
    writer.EndArray();

    writer.EndObject();
    std::ofstream outFile( "jsons/FunctionSets.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateExpensiveHeaderInfosJsonFile() {
    fprintf(out, "***********Generate Expensive Headers Json File*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Expensive Headers");

    writer.Key("Header Names");
    writer.StartArray();
    for (const auto &info : m_ExpensiveHeadersInfos)
    {
        writer.String(info.name.data());
    }
    writer.EndArray();

    writer.Key("Total Time");
    writer.StartArray();
    for (const auto &info : m_ExpensiveHeadersInfos)
    {
        writer.Int(info.total_time);
    }
    writer.EndArray();

    writer.Key("Average Time");
    writer.StartArray();
    for(const auto &info : m_ExpensiveHeadersInfos)
    {
        writer.Int(info.avg_time);
    }
    writer.EndArray();

    writer.Key("Included Times");
    writer.StartArray();
    for(const auto &info : m_ExpensiveHeadersInfos)
    {
        writer.Int(info.included_times);
    }
    writer.EndArray();

    writer.Key("Included Chains");
    writer.StartArray();
    for(const auto &info : m_ExpensiveHeadersInfos)
    {
        writer.StartArray();
        for (const auto &chains:info.included_chains)
        {
            writer.StartArray();
            for(const auto &chain_name:chains)
            {
                writer.String(chain_name.data());
            }
            writer.EndArray();
        }
        writer.EndArray();
    }
    writer.EndArray();

    writer.Key("Included Chains Time");

    writer.StartArray();
    for(const auto &info : m_ExpensiveHeadersInfos)
    {
        writer.StartArray();
        for (const auto &chain_time:info.included_chains_time)
        {
            writer.Int(chain_time);
        }
        writer.EndArray();
    }
    writer.EndArray();

    writer.EndObject();
    std::ofstream outFile( "jsons/ExpensiveHeaders.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

void CompilationInfo::GenerateWholeProcessInfo() {
    fprintf(out, "***********Generate Whole Compilation Process Info Json File*********\n");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("Type");
    writer.String("Whole Compilation Info");
    writer.Key("Total Time");
    writer.Int64(m_wholeProcessInfo.total_time);
    writer.Key("Parsing Time");
    writer.Int64(m_wholeProcessInfo.parsing_time);
    writer.Key("Code Gen&Opts Time");
    writer.Int64(m_wholeProcessInfo.code_gen_opts_time);

    writer.EndObject();
    std::ofstream outFile( "jsons/WholeCompilation.json");
    if(outFile.is_open())
    {
        outFile<<s.GetString();
        outFile.close();
    }
}

