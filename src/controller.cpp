#include "controller.hpp"


#include "randoms.hpp"


float determineLightness(sf::Color input)
{
    // https://thoughtbot.com/blog/closer-look-color-lightness
    // values have been normalized (ie divided by 255)
    return ((float)input.r) * 0.00083372549 +
	   ((float)input.g) * 0.00280470588 +
	   ((float)input.b) * 0.00028313725;
}

sf::Color randomColor(std::vector<float> lightRange)
{
    sf::Color result(0, 0, 0);
    do{
	result.r = randomI(0, 255);
	result.g = randomI(0, 255);
	result.b = randomI(0, 255);
    }while(determineLightness(result) > lightRange[0] && determineLightness(result) < lightRange[1]);

    return result;
}

Controller::Controller(std::shared_ptr<AsteroidSettings> aSetts, std::shared_ptr<ControllerSettings> cSetts):
    m_aSetts(aSetts), m_cSetts(cSetts)
{
    ;
}

void Controller::createAsteroid()
{
    /*
    m_asteroids.emplace_back(m_aSetts,
			     randomI(m_aSetts->m_massRange[0], m_aSetts->m_massRange[1]),
			     randomColor(m_aSetts->m_colorLightRange),
			     );
    */
}
