
void FileSys_Init(void);

void FileSys_Format(void);

void FileSys_PrintInfo(void);

File FileSys_OpenFile(const char* path, const char* mode);

void FileSys_CloseFile(File * pFile);