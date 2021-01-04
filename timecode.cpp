#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <exception>

class Timecode
{
private:
	long long frame_number;
	short frame_rate;

public:
	Timecode(const long long& fc, const float& fr = 23.98)
	{
		if (fr <= 0)
		{
			throw std::exception("Frame rate must be a positive non-zero value.");
		}
		else if (fc < 0)
		{
			throw std::exception("Negative timecodes are not yet supported.");
		}
		
		frame_number = fc;
		frame_rate = round(fr);
	}

	long long getFrameNumber() const
	{
		return frame_number;
	}

	short getFrames() const
	{
		return frame_number % frame_rate;
	}

	short getSeconds() const
	{
		return frame_number / frame_rate % 60;
	}

	short getMinutes() const
	{
		return frame_number / frame_rate / 60 % 60;
	}

	short getHours() const
	{
		return frame_number / frame_rate / 60 / 60;
	}

	std::string getTimecode() const
	{
		std::ostringstream output;
		output << std::setw(2) << std::setfill('0') << getHours() << ":" << std::setw(2) << getMinutes() << ":" << std::setw(2) << getSeconds() << ":" << std::setw(2) << getFrames();
		return output.str();
	}

	friend std::ostream& operator<< (std::ostream& stream, const Timecode& tc)
	{
		stream << tc.getTimecode();
		return stream;
	}
};

int main()
{
	for (unsigned long int x = 0; x < 240; ++x)
	{
		Timecode tc = Timecode(x);
		std::cout << "Timecode for " << tc.getFrameNumber() << " is " << tc << std::endl;
	}
}