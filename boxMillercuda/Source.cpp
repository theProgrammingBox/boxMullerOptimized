#include <iostream>
#include <chrono>

float InvSqrt(float number)
{
    long i = 0x5F1FFFF9 - (*(long*)&number >> 1);
    float tmp = *(float*)&i;
    return tmp * 0.703952253f * (2.38924456f - number * tmp * tmp);
}

std::pair<double, double> marsaglia_polar(int32_t* seed)
{
    double x, y, sqrMag;
    do {
        *seed ^= *seed << 13;
        *seed ^= *seed >> 17;
        *seed ^= *seed << 5;
        x = *seed * 4.6566128752457969e-10;
        *seed ^= *seed << 13;
        *seed ^= *seed >> 17;
        *seed ^= *seed << 5;
        y = *seed * 4.6566128752457969e-10;
        sqrMag = x * x + y * y;
    } while (sqrMag >= 1.0 || sqrMag == 0.0);

    double logSqrMag = std::log(sqrMag);
    double factor = InvSqrt(-0.5 * sqrMag / logSqrMag);
    double z1 = x * factor;
    double z2 = y * factor;

    return std::make_pair(z1, z2);
}

int main()
{
    const uint32_t samples = 10000000;
    const uint32_t bins = 100;
    const uint32_t halfBins = bins * 0.5;
    uint32_t histogram[bins] = { 0 };
    const double histogramScale = 1.0 / samples * 1000.0;
    const double mean = 0;
    const double stddev = 1;
    int32_t seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();


    for (int i = 0; i < samples; i++)
    {
        auto z = marsaglia_polar(&seed);
        uint32_t index = (z.first * stddev + mean) * 10.0 + halfBins;
        if (index >= 0 && index < bins)
			histogram[index]++;
        index = (z.second * stddev + mean) * 10.0 + halfBins;
        if (index >= 0 && index < bins)
			histogram[index]++;
	}

    for (int i = 0; i < bins; i++)
    {
        std::string sqrMag(histogram[i] * histogramScale, ' ');
        printf("%s*\n", sqrMag.c_str());
	}

    return 0;
}