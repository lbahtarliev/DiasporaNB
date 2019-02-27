// ---------------------------------------------------------------------------
#if !defined __CLIPPUT_I
#define __CLIPPUT_I
// Copy input to Windows clipboard
// Data lines must be terminated by the CR LF pair (0xD,0xA)
// data in: line1CRLFline2CRLFline3CRLF --- Caller must format
// "this is a line\n" is not acceptable,
// "this is a line\r\n" is acceptable.
// If clipboard data shows square empty boxes at line ends in Windows,
// it is because lines are terminated by \n only.
#include <windows.h>
int clipput(char *toclipdata)
{
    char far *buffer;
    int bytes;
    
    HGLOBAL clipbuffer;
    
    // Set data buffer length to accomodate toclipdata
    bytes = strlen(toclipdata);
    
    // Peform the transfer to clipboard magic
    OpenClipboard(NULL);
    EmptyClipboard();
    clipbuffer = GlobalAlloc(GMEM_DDESHARE,bytes+1);
    buffer = (char far*)GlobalLock(clipbuffer);
    if (buffer == NULL)
       return GetLastError() * -1; // Do what you want to signal error
    strcpy(buffer,toclipdata);
    
    GlobalUnlock(clipbuffer);
    SetClipboardData(CF_TEXT,clipbuffer);
    CloseClipboard();
    
    // Return byte count
    return bytes; // non-negative value is success.
}
// ---------------------------------------------------------------------------
#if !defined __CLIPGET_I
#define __CLIPGET_I
#include <windows.h>
// Return pointer to clipboard data and set bytes returned value
// If error occurs, set up error message, point to it, set bytes negative
// Whether successful or not, the caller SHOULD free the data

char *nigh_clipget(char *data, int max_size)
{
    data[0] = 0;
    
    if (OpenClipboard(NULL))
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char * buffer = (char*)GlobalLock(hData);
        GlobalUnlock(hData);
        CloseClipboard();
        
        if (buffer)
            strncpy(data, buffer, max_size);
    }
    
    return data;
}


char *clipget(int &bytes)
{
    int k;
    char *buffer=NULL;
    char *data=NULL;
    char empty[80]="<Clipboard is empty>";
    
    bytes = 0;
    
    // open the clipboard
    if (OpenClipboard(NULL))
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char * buffer = (char*)GlobalLock(hData);
        GlobalUnlock(hData);
        CloseClipboard();
        
        // Return an error message
        if (buffer == NULL)
        {
            bytes = strlen(empty);
            data = (char *) malloc(bytes+1);
            strcpy(data,empty);
            bytes = bytes * -1;
        }
        // Return pointer to retrieved data
        else
        {
            bytes = strlen(buffer);
            data = (char *) malloc(bytes+1);
            strcpy(data,buffer);
        }
    }
    // Return an open clipboard failed message
    else
    {
        k = GetLastError();
        if (k < 0)
            bytes = k;
        else
            bytes = k * -1;
        sprintf(empty,"Error occurred opening clipboard - RC: %d",k);
        k = strlen(empty);
        data = (char *) malloc(k+1);
        strcpy(data,empty);
    }
    // Return pointer to data field allocated 
    // It's up to the caller to free the storage
    return data;
}
#endif

#endif
