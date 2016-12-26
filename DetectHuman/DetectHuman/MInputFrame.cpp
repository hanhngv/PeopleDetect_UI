#include "stdafx.h"
#include "MInputFrame.h"


UINT RunFrameThreadInput(LPVOID pdata){
	MInputFrame* model = (MInputFrame*)pdata;
	model->inputFrameExcutor();

	return 0;
}

MInputFrame::MInputFrame(){
	m_is_has_new_frame = false;
	m_is_ready = false;
	m_remain_frame = false;
	m_input_thread = NULL;
}

MInputFrame::~MInputFrame(){
	if(m_input_thread){
		m_input_thread->Delete();
		m_input_thread = NULL;
	}
}

void MInputFrame::addFrame(){
	Mat new_frame;
	m_remain_frame = addOneFrame(new_frame);

	time_t cur_time = clock();

	CSingleLock lock(&m_sync_input_frame);
	while(lock.IsLocked())
		Sleep(1);
	lock.Lock();

	if(lock.IsLocked()){
		m_last_frame = m_cur_frame.clone();
		m_cur_frame = new_frame.clone();
		m_is_has_new_frame = true;
		m_cur_time = cur_time;
	}

	lock.Unlock();
}

bool MInputFrame::getFrame(Mat& cur_frame, Mat& last_frame, time_t &cur_update_time){
	CSingleLock lock(&m_sync_input_frame);
	while(lock.IsLocked())
		Sleep(1);
	lock.Lock();

	if(lock.IsLocked()){
		if(m_is_has_new_frame){
			cur_frame = m_cur_frame.clone();
			last_frame = m_last_frame.clone();
		}
		else{
			cur_frame = Mat(0, 0, CV_8UC3);
			last_frame = Mat(0, 0, CV_8UC3);
		}
		cur_update_time = m_cur_time;

		m_is_has_new_frame = false;
	}

	lock.Unlock();

	return m_remain_frame;
}

void MInputFrame::inputFrameExcutor(){
	while(m_remain_frame){
		addFrame();
		Sleep(30);
	}
}

void MInputFrame::stop(){
	m_remain_frame;
}

cv::Size MInputFrame::getFrameSize(){
	return m_frame_size;
}

bool MInputFrame::isReady(){
	bool is_ready;
	CSingleLock lock(&m_sync_status);
	lock.Lock();

	is_ready = m_is_ready;

	lock.Unlock();

	return m_is_ready;
}

/// -----------------------------------------------------
/// ----------------------- MInputFromVideo -------------
/// -----------------------------------------------------

bool MInputFromVideo::init(string file_name){
	m_capture = VideoCapture(file_name);

	if(!m_capture.isOpened())
		return false;

	m_remain_frame = true;
		
	Mat tmp_frame;
	m_capture >> tmp_frame;

	m_frame_size = tmp_frame.size();
	if(m_frame_size.width > 0)
		m_is_ready = true;

	m_input_thread = AfxBeginThread(RunFrameThreadInput, this);	

	return true;
}

/// -----------------------------------------------------
/// ----------------------- MInputFromCamera -------------
/// -----------------------------------------------------
//UINT RunFrameThreadConnectCamera(LPVOID pdata){
//	MInputFrame* model = (MInputFrame*)pdata;
//	model->inputFrameExcutor();
//
//	return 0;
//}

bool MInputFrameFormCamera::addOneFrame(Mat& new_frame){
	if(m_remain_frame){
		m_capture >> new_frame;
        if(new_frame.empty())
            m_remain_frame = false;
	}
	else
		new_frame = Mat(0, 0, CV_8UC3);

	return m_remain_frame;
}


bool MInputFrameFormCamera::init(string stream_address){
	m_capture.open(stream_address);

	if(!m_capture.isOpened())
		return false;

	m_remain_frame = true;
		
	Mat tmp_frame;
	m_capture >> tmp_frame;

	m_frame_size = tmp_frame.size();
	if(m_frame_size.width > 0)
		m_is_ready = true;

	m_input_thread = AfxBeginThread(RunFrameThreadInput, this);	

	return true;
}

bool MInputFromVideo::addOneFrame(Mat& new_frame){
	if(m_remain_frame){
		m_capture >> new_frame;
        if(new_frame.empty())
            m_remain_frame = false;
	}
	else
		new_frame = Mat(0, 0, CV_8UC3);

	return m_remain_frame;
}