#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <exception>

#include <set>

class Timecode
{
private:
	long frame_number;
	short frame_rate;
	bool drop_frame;

	bool isCompatible(const Timecode& comp) const
	{
		return (comp.getRate() == getRate()) && (comp.isDropFrame() == isDropFrame());
	}

public:
	Timecode(const long& fc, const float& fr = 23.98)
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
		drop_frame = false; // for now
	}

	short getRate() const
	{
		return frame_rate;
	}

	long getFrameNumber() const
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

	bool isDropFrame() const
	{
		return drop_frame;
	}

	bool operator== (const Timecode& comp) const
	{
		return isCompatible(comp) && (comp.getFrameNumber() == getFrameNumber());
	}

	bool operator!= (const Timecode& comp) const
	{
		return isCompatible(comp) && (comp.getFrameNumber() != getFrameNumber());
	}

	bool operator< (const Timecode& comp) const
	{
		if (isCompatible(comp))
			return getFrameNumber() < comp.getFrameNumber();
		else
			return getRate() < comp.getRate();
	}

	bool operator> (const Timecode& comp) const
	{
		if (isCompatible(comp))
			return getFrameNumber() > comp.getFrameNumber();
		else
			return getRate() < comp.getRate();
	}



	Timecode operator+(const Timecode& comp)
	{
		if (!isCompatible(comp))
			throw std::exception("Timecodes must be the same rate");

		return Timecode(comp.getFrameNumber() + getFrameNumber(), getRate());
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
	using namespace std;
	set<Timecode> timecodes;


	for (short x = 30; x > 0; --x)
	{
		timecodes.insert(Timecode(86400 + x));
		timecodes.insert(Timecode(120600 + x, 30));
	}
	cout << endl << "---------" << endl;

	//timecodes.sort();
	
	cout << "List has " << timecodes.size() << " elements:" << endl;
	for (const auto& tc : timecodes)
	{
		cout << tc << " @ " << tc.getRate() << endl;
	}

	return 0;
}