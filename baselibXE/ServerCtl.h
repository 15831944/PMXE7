// ---------------------------------------------------------------------------

#ifndef ServerCtlH
#define ServerCtlH
// ---------------------------------------------------------------------------
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCCreateService(Char *lpComputerName, Char *lpServiceName,
	Char *lpDisplayName, Char *lpFileName);
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCDeleteService(Char *lpComputerName, Char *lpServiceName);
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCStartService(Char *lpComputerName, Char *lpServiceName);
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCStopService(Char *lpComputerName, Char *lpServiceName);
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCPauseService(Char *lpComputerName, Char *lpServiceName);
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCResumeService(Char *lpComputerName, Char *lpServiceName);
// �ɹ�����0,ʧ�ܷ��ط�0
extern PACKAGE int SCQueryServiceStatus(Char *lpComputerName,
	Char *lpServiceName, int &lpServiceStatus);
#endif
