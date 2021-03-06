#include "MerryApp.h"

IMPLEMENT_APP(MerryApp)

bool MerryApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	#ifdef __WXMSW__
	wxStandardPaths std; //<wx/stdpaths.h>
	wxFileName fname = wxFileName(std.GetExecutablePath());
	wxString volume;
	wxString pathTmp = fname.GetPathWithSep(); //<wx/filename.h>
	::wxSetEnv(wxT("ALMRUN_HOME"),pathTmp.c_str());
	wxFileName::SplitVolume(pathTmp,&volume,NULL);
	if (!volume.empty())
	{
		volume.Append(':');
		::wxSetEnv(wxT("ALMRUN_DRIVE"),volume.c_str());
	}
	::wxSetEnv(wxT("ALMRUN_ROOT"),pathTmp.c_str());
	::wxSetWorkingDirectory(pathTmp);
	pathTmp.Clear();
	volume.Clear();
	#endif
	m_frame = NULL;
	this->NewFrame();
	assert(m_frame);

	return true;
}

void MerryApp::NewFrame()
{
	if (m_frame)
	{
		bool ok = m_frame->Close();
		assert(ok);
	}
	
	m_frame = new MerryFrame();
	m_frame->OnInit();
}

MerryFrame& MerryApp::GetFrame()
{
	assert(m_frame);
	return *m_frame;
}
