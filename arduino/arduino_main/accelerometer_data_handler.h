#ifndef ACCELEROMETER_DATA_HANDLER_H
#define ACCELEROMETER_DATA_HANDLER_H
/*
#include <stdbool.h>
#include <math.h>
#include <queue.h>

constexpr std::size_t SAMPLE_RATE = 10; // not used here
constexpr std::size_t THRESHOLD = 2; // will have to be fine tuned

struct AcceleratorData
{
	float x;
	float y;
	float z;
};

std::queue<struct AcceleratorData> queue;
struct AcceleratorData previous_calibrated_data;


// temporary method with mock values
struct AcceleratorData get_accelerator_data()
{
	struct AcceleratorData raw_data;
	raw_data.x = 1;
	raw_data.y = 2;
	raw_data.z = 3;
	return raw_data;
}

float calculate_vector_sum(struct AcceleratorData current, struct AcceleratorData previous)
{
	float x = current.x - previous.x;
	float y = current.y - previous.y;
	float z = current.z - previous.z;

	return sqrt(x - y - z);
}

//calibrates raw data
struct AcceleratorData calibrate_data()
	{
	

	float x_sum = 0.0f;
	float y_sum = 0.0f;
	float z_sum = 0.0f;

	std::queue<struct AcceleratorData> copy = queue;

	while (!copy.empty())
	{
		struct AcceleratorData ad = copy.front();
		x_sum += ad.x;
		y_sum += ad.y;
		z_sum += ad.z;
		copy.pop();
	}

	float x1_avg = x_sum / queue.size();
	float y1_avg = y_sum / queue.size();
	float z1_avg = z_sum / queue.size();

	float x_even_sum = 0.0f;
	float x_odd_sum = 0.0f;

	float y_even_sum = 0.0f;
	float y_odd_sum = 0.0f;

	float z_even_sum = 0.0f;
	float z_odd_sum = 0.0f;

	int i = 1;

	copy = queue;

	while (!copy.empty())
	{
		struct AcceleratorData ad = copy.front();
		if (i % 2 == 0)
		{
			x_even_sum += ad.x;
			y_even_sum += ad.y;
			z_even_sum += ad.z;
		}

		else
		{
			x_odd_sum += ad.x;
			y_odd_sum += ad.y;
			z_odd_sum += ad.z;
		}
		copy.pop();
		i++;
	}

	int half_queue_size = queue.size() / 2;

	float x2_avg = (x_odd_sum / half_queue_size) + (x_even_sum / half_queue_size);
	float y2_avg = (y_odd_sum / half_queue_size) + (y_even_sum / half_queue_size);
	float z2_avg = (z_odd_sum / half_queue_size) + (z_even_sum / half_queue_size);

	float x = (x1_avg + x2_avg) / 2;
	float y = (y1_avg + y2_avg) / 2;
	float z = (z1_avg + z2_avg) / 2;

	struct AcceleratorData calibrated_data;

	calibrated_data.x = x;
	calibrated_data.y = y;
	calibrated_data.z = z;

	return calibrated_data;
}

// main collision detection method
bool detect_collision()

{

	struct AcceleratorData raw_data = get_accelerator_data();


	queue.push(raw_data);

	if (queue.size() >= 6)
	{
		queue.pop();
	}

	struct AcceleratorData current_calibrated_data = calibrate_data();
	float vector_sum = calculate_vector_sum(current_calibrated_data, previous_calibrated_data);

	previous_calibrated_data = current_calibrated_data;

	if (vector_sum > THRESHOLD)
	{

		return true;
	
	}

	return false;

}

*/

#endif