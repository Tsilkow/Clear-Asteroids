#include "scores.hpp"

#include <iomanip>
#include <fstream>


using namespace nlohmann;

Scores::Scores(int scoresKept):
    m_scoresKept(scoresKept)
{
    ;
}

bool Scores::load(std::string filename)
{
    std::fstream file(filename, std::fstream::in);

    if(!file.good()) return false;

    m_scores.clear();

    json parse = json::parse(file);
    std::cout << std::setw(4) << parse << std::endl;

    try{
	for(int i = 0; i < parse["scores"].size() && i < m_scoresKept; ++i)
	{
	    m_scores.emplace_back(std::pair<std::string, int>(parse["scores"][i][0].get<std::string>(),
							      parse["scores"][i][1].get<int>()));
	}
    }
    catch(json::exception& e)
    {
	std::cout << "Score file error = " << e.what() << std::endl;
    }
    
    file.close();

    return true;
}


bool Scores::save(std::string filename)
{
    std::fstream file(filename, std::fstream::out);

    if(!file.good()) return false;

    json parse;

    parse["scores"] = {};

    for(int i = 0; i < m_scores.size(); ++i)
    {
	parse["scores"][i][0] = m_scores[i].first;
	parse["scores"][i][1] = m_scores[i].second;
    }
    
    std::cout << std::setw(4) << parse << std::endl;

    file << parse;
    file.close();

    return true;
}
