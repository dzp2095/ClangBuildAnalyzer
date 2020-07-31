//
// Created by dengzhipeng on 2020/7/28.
//

#ifndef CLANGBUILDANALYZER_COMPILATIONINFO_H
#define CLANGBUILDANALYZER_COMPILATIONINFO_H

#include <utility>

#include "BuildEvents.h"


struct ParseFileInfo
{
    ParseFileInfo(std::string_view name, int time_ms): name(name), time_ms(time_ms){};
    std::string_view name;
    int time_ms;
};

struct CodegenFileInfo
{
    CodegenFileInfo(std::string_view name, int time_ms): name(name), time_ms(time_ms){};
    std::string_view name;
    int time_ms;
};

struct TemplateInfo
{
    TemplateInfo(std::string_view name, int time_ms, int avg_time, int ins_times): name(name), time_ms(time_ms), avg_time(avg_time), ins_times(ins_times){};
    std::string_view name;
    int time_ms;
    int avg_time;
    int ins_times;
};

struct TemplateSetInfo
{
    TemplateSetInfo(std::string_view name, int time_ms, int avg_time, int ins_times): name(name), time_ms(time_ms), avg_time(avg_time), ins_times(ins_times){};
    std::string_view name;
    int time_ms;
    int avg_time;
    int ins_times;
};

struct FunctionInfo
{
    FunctionInfo(std::string_view func_name,std::string_view file_name,int time_ms):func_name(func_name),file_name(file_name),time_ms(time_ms){};
    std::string_view func_name;
    std::string_view file_name;
    int time_ms;
};

struct FunctionSetInfo
{
    FunctionSetInfo(std::string_view name, int time_ms, int avg_time, int ins_times): name(name), time_ms(time_ms), avg_time(avg_time), ins_times(ins_times){};
    std::string_view name;
    int time_ms;
    int avg_time;
    int ins_times;
};

struct ExpensiveHeaderInfo
{
    ExpensiveHeaderInfo(const std::string_view &name, int includedTimes, int avgTime, int totalTime,
                        std::vector<std::vector<std::string_view>> includedChains,
                        std::vector<int> includedChainsTime) : name(name), included_times(includedTimes),
                                                                      avg_time(avgTime), total_time(totalTime),
                                                                      included_chains(std::move(includedChains)),
                                                                      included_chains_time(std::move(includedChainsTime)) {}

    std::string_view name;
    int included_times;
    int avg_time;
    int total_time;
    std::vector<std::vector<std::string_view> > included_chains;
    std::vector<int> included_chains_time;
};

struct WholeProcessInfo
{

    WholeProcessInfo()= default;

    WholeProcessInfo(int64_t totalTime, int64_t parsingTime, int64_t codeGenOptsTime) : total_time(totalTime),
                                                                                        parsing_time(parsingTime),
                                                                                        code_gen_opts_time(
                                                                                                codeGenOptsTime) {}

    int64_t total_time{};
    int64_t parsing_time{};
    int64_t code_gen_opts_time{};
};

class CompilationInfo
{
public:
    explicit CompilationInfo(FILE *out, WholeProcessInfo mWholeProcessInfo);
    ~CompilationInfo();
    void AddParseFilesInfo(ParseFileInfo parseFilesInfo)
    {
        m_ParseFilesInfos.push_back(parseFilesInfo);
    }
    void AddCodegenFilesInfo(CodegenFileInfo codegenFilesInfo)
    {
        m_CodegenFilesInfos.push_back(codegenFilesInfo);
    }
    void AddTemplatesInfo(TemplateInfo instantiationsInfo)
    {
        m_TemplatesInfos.push_back(instantiationsInfo);
    }
    void AddTemplateSetsInfo(TemplateSetInfo templateSetsInfo)
    {
        m_TemplateSetsInfos.push_back(templateSetsInfo);
    }
    void AddFunctionInfo(FunctionInfo functionInfo)
    {
        m_FunctionInfos.push_back(functionInfo);
    }
    void AddFunctionSetInfo(FunctionSetInfo functionSetInfo)
    {
        m_FunctionSetInfos.push_back(functionSetInfo);
    }
    void AddExpensiveHeaderInfo(ExpensiveHeaderInfo expensiveHeaderInfo)
    {
        m_ExpensiveHeadersInfos.push_back(expensiveHeaderInfo);
    }
    void SetWholeProcessInfo(WholeProcessInfo wholeProcessInfo)
    {
        m_wholeProcessInfo = wholeProcessInfo;
    }
    void GenerateJsonFile();
private:
    std::vector<ParseFileInfo> m_ParseFilesInfos;
    std::vector<CodegenFileInfo> m_CodegenFilesInfos;
    std::vector<TemplateInfo> m_TemplatesInfos;
    std::vector<TemplateSetInfo> m_TemplateSetsInfos;
    std::vector<FunctionInfo> m_FunctionInfos;
    std::vector<FunctionSetInfo> m_FunctionSetInfos;
    std::vector<ExpensiveHeaderInfo> m_ExpensiveHeadersInfos;
    WholeProcessInfo m_wholeProcessInfo;

    void GenerateParseFilesJsonFile();
    void GenerateCodegenFilesJsonFile();
    void GenerateTemplatesJsonFile();
    void GenerateTemplateSetsJsonFile();
    void GenerateFunctionInfosJsonFile();
    void GenerateFunctionSetInfosJsonFile();
    void GenerateExpensiveHeaderInfosJsonFile();
    void GenerateWholeProcessInfo();
    FILE* out;
};

#endif //CLANGBUILDANALYZER_COMPILATIONINFO_H
