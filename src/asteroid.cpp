#include "asteroid.hpp"


#include "randoms.hpp"
#include <math.h>


float radToDeg(float radians)
{
    return radians * 180/M_PI;
}

float normalizeAngle(float angle)
{
    while(angle <       0) angle += 2*M_PI;
    while(angle >= 2*M_PI) angle -= 2*M_PI;
    return angle;
}

void connectPeaks(std::vector<float>& result, int begin, int end, float amplitude, int loop)
{
    // if the peaks are too close
    if(end - begin < 2) return;

    int middle = (begin + end)/2;
    ++loop;

    result[middle] = (result[begin] + result[end])/2.f +
	randomF(-amplitude/((float)loop), amplitude/((float)loop));
    connectPeaks(result, begin, middle, amplitude, loop);
    connectPeaks(result, middle,   end, amplitude, loop);
}

std::vector<float> generateHeights(int TOSegments, float amplitude, int peakFrequency, int peakVariation, float peakAmplitude)
{
    std::vector<float> result(TOSegments);
    std::vector<int> peaks;
    int peakIndex = 0;

    peaks.emplace_back(0);
    result[peaks.back()] = 1.f;
    // generating peaks
    for(int i = 1; i < TOSegments/peakFrequency; ++i)
    {
	peaks.emplace_back(peakFrequency * i + randomI(-peakVariation, peakVariation));
	result[peaks.back()] = 1.f + randomF(-peakAmplitude, peakAmplitude);
    }
    // repeat the first peak, so it will be taken into account when connecting
    peaks.emplace_back(TOSegments);
    result.emplace_back(1.f);

    // generating heights in between peaks
    for(int i = 1; i < peaks.size(); ++i)
    {
	connectPeaks(result, peaks[i-1], peaks[i], amplitude, 0);
    }

    return result;
}

Asteroid::Asteroid(const std::shared_ptr<AsteroidSettings> aSetts, int mass, sf::Color color, sf::Vector2f position, sf::Vector2f velocity, float angVelocity):
    m_mass(mass),
    m_color(color),
    m_position(position),
    m_angle(0.f),
    m_velocity(velocity),
    m_angVelocity(angVelocity)
{
    std::vector<float> heights = generateHeights(aSetts->m_TOSegments, aSetts->m_amplitude, aSetts->m_peakFrequency, aSetts->m_peakVariation, aSetts->m_peakAmplitude);

    m_radius = sqrt(((float)mass) * 1/M_PI * aSetts->m_invDensity);

    // creating the actual, spherical, but still static, representation from heights
    for(int i = 0; i < heights.size(); ++i)
    {
	float angle = ((float)i)/((float)heights.size()) * 2*M_PI;
	m_staRepres.emplace_back(sf::Vector2f(cos(angle), sin(angle)) * heights[i] * ((float)m_radius),
				m_color);
    }
    // repeat the first height at the beginning, so that the shapes closes
    m_staRepres.emplace_back(sf::Vector2f(1.f, 0.f) * heights[0] * ((float)m_radius));

    // copy positions of static representation verticies and move them by position of the asteroid
    for(int i = 0; i < m_staRepres.size(); ++i)
    {
	m_dynRepres.emplace_back(m_staRepres[i].position + m_position, m_staRepres[i].color);
    }
    //m_collisionMask.setRadius(m_radius);
    //m_collisionMask.setPosition(m_position);
}

bool Asteroid::tick(sf::FloatRect bounds)
{
    m_velocity += m_force * 1.f/(float)m_mass;
    m_force = sf::Vector2f(0.f, 0.f);
    m_position += m_velocity;
    m_angle = normalizeAngle(m_angle + m_angVelocity);

    // take positions of static representation verticies, rotate them and move them
    sf::Transform R;
    R.rotate(radToDeg(m_angle));
    for(int i = 0; i < m_staRepres.size(); ++i)
    {
	m_dynRepres[i].position = R.transformPoint(m_staRepres[i].position) + m_position;
    }
    
    //m_collisionMask.setPosition(m_position - sf::Vector2f(1.f, 1.f) * (float)m_radius);

    return bounds.contains(m_position.x, m_position.y);
}

void Asteroid::draw(sf::RenderTarget& target)
{
    target.draw(&m_dynRepres[0], m_dynRepres.size(), sf::TriangleFan);
    //target.draw(m_collisionMask);
}
