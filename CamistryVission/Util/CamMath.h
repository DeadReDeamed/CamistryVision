#ifndef CAM_MATH_H
#define CAM_MATH_H

namespace camvis
{
	class CamMath
	{
	public:
		template<typename T>
		static T lerp(T current, T target, T percentage)
		{
			if (percentage < 0 || percentage > 1) // Invalid percentage
			{
				return current;
			}

			return current + percentage * (target - current);
		}
	};
}

#endif
