#pragma once
#include "MInputFrame.h"
class CMyVideo: public MInputFromVideo
{
protected:
	CCriticalSection m_syn;
public:
	CMyVideo(void);
	~CMyVideo(void);
	virtual bool init(string file_name)
	{
		return MInputFromVideo::init(file_name);
	};
	virtual bool addOneFrame(Mat& new_frame)
	{
		CSingleLock lock(&m_sync_input_frame);
		while(lock.IsLocked())
			Sleep(1);
		lock.Lock();
		bool res = MInputFromVideo::addOneFrame(new_frame);
		lock.Unlock();
		return res;
	};
	bool GetOneFrame()
	{
		return addOneFrame(m_image);
	};
	Mat GetImage()
	{
		CSingleLock lock(&m_sync_input_frame);
		while(lock.IsLocked())
			Sleep(1);
		lock.Lock();
		Mat timage = m_image;
		lock.Unlock();
		return timage;
	};
private:
	Mat m_image;
};
