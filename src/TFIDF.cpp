#include "TFIDF.h"
#include <math.h> // log

TFIDF::TFIDF(const std::vector<FileInfo> &filesInfo)
{
    for (const auto &fileInfo : filesInfo)
    {
        pathToIndex[fileInfo.getPath()] = TFMetrics.size();
        TFMetrics.push_back(FileInfo(fileInfo.getPath()));
        for (const auto &[word, count] : fileInfo.getAllMetric())
        {
            TFMetrics.back().setMetric(word, count / fileInfo.getAmountOfWords());
            IDFMetrics[word] += 1;
        }
    }

    for (auto &[word, count] : IDFMetrics)
    {
        count = std::log(filesInfo.size() / count);
    }

    for (auto &[word, count] : IDFMetrics)
    {
        setUnicWords.insert(word);
    }
}

int TFIDF::countUnicWords() const
{
    return setUnicWords.size();
}

std::set<std::string> TFIDF::getSetUnicWords() const
{
    return setUnicWords;
}

double TFIDF::calculate(const std::string &word, const std::string &path) const
{
    try
    {
        return TFMetrics.at(pathToIndex.at(path)).getAllMetric().at(word) * IDFMetrics.at(word);
    }
    catch (const std::out_of_range &e)
    {
        return 0;
    }
}

FileInfo TFIDF::TFIDFForFile(const std::string &path) const
{
    FileInfo fileInfo(path);
    for (auto &[word, count] : TFMetrics.at(pathToIndex.at(path)).getAllMetric())
    {
        fileInfo.setMetric(word, count);
    }
    return fileInfo;
}