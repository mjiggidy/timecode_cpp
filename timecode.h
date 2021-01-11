#pragma once

class Timecode
{
private:
	long  m_frame_number;
	short m_frame_rate;
	bool  m_drop_frame;

	bool Validate();
	bool isCompatible(const Timecode&) const;
	int  dropFrameOffset() const;

public:
	Timecode(const long& fc, const float& = 24, bool = false);
	Timecode(const std::string&, const float& = 24, bool = false);

	long  getFrameNumber() const;
	short getRate()        const;
	bool  isDropFrame()    const;

	short getFrames()  const;
	short getSeconds() const;
	short getMinutes() const;
	short getHours()   const;

	bool operator== (const Timecode&) const;
	bool operator!= (const Timecode&) const;
	bool operator<  (const Timecode&) const;
	bool operator>  (const Timecode&) const;

	Timecode operator+ (const Timecode&);
	Timecode operator- (const Timecode&);
	
	friend std::ostream& operator<< (std::ostream&, const Timecode&);

	std::string getTimecode() const;
};