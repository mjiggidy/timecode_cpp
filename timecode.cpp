#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <iomanip>
#include <exception>
#include <deque>
#include "timecode.h"


bool Timecode::isCompatible(const Timecode& comp) const
{
	return (comp.getRate() == getRate()) && (comp.isDropFrame() == isDropFrame());
}

int Timecode::dropFrameOffset() const
{
	// Two frames of timecode are dropped every minute, except on the tenth minute, when nothing is dropped

	return 0;

}

Timecode::Timecode(const long& fc, const float& fr, bool dropframe) : m_frame_number(fc), m_frame_rate(round(fr)), m_drop_frame(dropframe)
{
	Validate();
}

Timecode::Timecode(const std::string& tc, const float& fr, bool dropframe) : m_frame_number(0), m_frame_rate(round(fr)), m_drop_frame(dropframe)
{
	m_frame_rate = round(fr);
	m_drop_frame = dropframe;
	
	// Validate
	std::regex re(R"([+\-]?\d+(?::(\d+)){0,3})");
	std::smatch m;
	
	if (!std::regex_match(tc, m, re))
	{
		throw std::exception("Invalid timecode provided");
	}

	bool is_neg = false;
	std::deque<int> nums;
	size_t seg_start = 0;

	if (tc[0] == '+')
	{
		++seg_start;
	}
	else if (tc[0] == '-')
	{
		is_neg = true;
		++seg_start;
	}

	size_t last = seg_start;
	size_t next = 0;
	while ((next = tc.find(":", last)) != std::string::npos)
	{
		int num = std::stoi(tc.substr(last, next - last));
		nums.push_front(num);
		last = next + 1;
	}
	nums.push_front(std::stoi(tc.substr(last)));

	nums.resize(4);


	m_frame_number += nums[0];
	m_frame_number += nums[1] * m_frame_rate;
	m_frame_number += nums[2] * m_frame_rate * 60;
	m_frame_number += nums[3] * m_frame_rate * 60 * 60;

	Validate();
}

bool Timecode::Validate()
{
	if (m_frame_rate <= 0)
	{
		throw std::exception("Frame rate must be a positive non-zero value.");
	}
	else if (m_frame_number < 0)
	{
		throw std::exception("Negative timecodes are not yet supported.");
	}

	if (m_drop_frame && (m_frame_rate % 30))
		throw std::exception("Drop frame is only supported for multiples of 30fps.");
	
	return true;
}

short Timecode::getRate() const
{
	return m_frame_rate;
}

long Timecode::getFrameNumber() const
{
	return m_frame_number;
}

short Timecode::getFrames() const
{
	return (m_frame_number + dropFrameOffset()) % m_frame_rate;
}

short Timecode::getSeconds() const
{
	return (m_frame_number + dropFrameOffset()) / m_frame_rate % 60;
}

short Timecode::getMinutes() const
{
	return (m_frame_number + dropFrameOffset()) / m_frame_rate / 60 % 60;
}

short Timecode::getHours() const
{
	return (m_frame_number + dropFrameOffset()) / m_frame_rate / 60 / 60;
}

bool Timecode::isDropFrame() const
{
	return m_drop_frame;
}

bool Timecode::operator== (const Timecode& comp) const
{
	return isCompatible(comp) && (comp.getFrameNumber() == getFrameNumber());
}

bool Timecode::operator!= (const Timecode& comp) const
{
	return isCompatible(comp) && (comp.getFrameNumber() != getFrameNumber());
}

bool Timecode::operator< (const Timecode& comp) const
{
	if (isCompatible(comp))
		return getFrameNumber() < comp.getFrameNumber();
	else
		return getRate() < comp.getRate();
}

bool Timecode::operator> (const Timecode& comp) const
{
	if (isCompatible(comp))
		return getFrameNumber() > comp.getFrameNumber();
	else
		return getRate() < comp.getRate();
}



Timecode Timecode::operator+(const Timecode& opr)
{
	if (!isCompatible(opr))
		throw std::exception("Timecodes must be the same rate");

	return Timecode(getFrameNumber() + opr.getFrameNumber(), getRate(), isDropFrame());
}

Timecode Timecode::operator-(const Timecode& opr)
{
	if (!isCompatible(opr))
		throw std::exception("Timecodes must be the same rate");

	return Timecode(getFrameNumber() - opr.getFrameNumber(), getRate(), isDropFrame());
}

std::string Timecode::getTimecode() const
{
	std::ostringstream output;
	output << std::setw(2) << std::setfill('0') << getHours() << ":" << std::setw(2) << getMinutes() << ":" << std::setw(2) << getSeconds() << (isDropFrame() ? ";" : ":") << std::setw(2) << getFrames();
	return output.str();
}

std::ostream& operator<< (std::ostream& stream, const Timecode& tc)
{
	stream << tc.getTimecode();
	return stream;
}

