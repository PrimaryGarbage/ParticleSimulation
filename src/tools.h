#pragma once
#include "SFML/Graphics.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include <math.h>
#include <iostream>

namespace chonk {

	template<typename T>
	inline sf::Vector2<T> glm_to_sf_vec(const glm::vec<2, T>& vec)
	{
		return sf::Vector2<T>(vec.x, vec.y);
	}


	template<typename T>
	inline glm::vec<2, T> sf_to_glm_vec(const sf::Vector2<T>& vec)
	{
		return glm::vec<2, T>(vec.x, vec.y);
	}


	template<typename T>
	inline glm::vec<2, T> clampVec(const glm::vec<2, T>& vec, const float minVal, const float maxVal)
	{
		float newLength = glm::clamp(glm::length(vec), minVal, maxVal);
		if(newLength == 0.0f)
            return glm::vec2();
        else
            return glm::normalize(vec) * newLength;
	}


    float sfLength(const sf::Vector2f& vec)
	{
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
	}


    sf::Vector2f sfNormalize(const sf::Vector2f& vec)
    {
        float length = sfLength(vec);
        if(length > 0.0f)
            return sf::Vector2f(vec) / length;
        else
        {
            //std::cerr << "chonk::sfNormalize: zero division" << std::endl;
            return sf::Vector2f(0.0f, 0.0f);
        }
    }


    inline sf::Vector2f clampVec(const sf::Vector2f vec, const float minVal, const float maxVal)
    {
        float c = sfLength(vec);
        if(c > 0.0f)
        {
            float newLength = glm::clamp(c, minVal, maxVal);
            return sf::Vector2f(vec / c) * newLength;
        }
        else
        {
            //std::cerr << "chonk::clampVec: zero division" << std::endl;
            return sf::Vector2f(0.0f, 0.0f);
        }
    }


	inline float lerp(float a, float b, float ratio)
	{
		return (b - a) * ratio + a;
	}


	inline float lerpAngle(float a, float b, float ratio)
	{
		float result;
		float diff = b - a;
		if (diff < -180.f)
		{
			// lerp upwards past 360
			b += 360.f;
			result = lerp(a, b, ratio);
			if (result >= 360.f)
			{
				result -= 360.f;
			}
		}
		else if (diff > 180.f)
		{
			// lerp downwards past 0
			b -= 360.f;
			result = lerp(a, b, ratio);
			if (result < 0.f)
			{
				result += 360.f;
			}
		}
		else
		{
			// straight lerp
			result = lerp(a, b, ratio);
		}

		return result;
	}
}
