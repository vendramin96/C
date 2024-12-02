#include "platform.h"

#ifdef WINDOWS_OS
    #include <windows.h>
#else
    #error
#endif

typedef u8* va_args;
#define VAGet(argument, type) *(type *)VAGet_(argument)

#include "Code/string.c"

internal_function va_args
VABegin(void *Format)
{
    va_args Result = 0;

    Result = (u8 *)Format + POINTER_SIZE;
    return Result;
}

internal_function va_args
VAEnd()
{
    va_args Result = 0;

    return Result;
}

internal_function void *
VAGet_(va_args *Argument)
{
    void *Result = *Argument;

    *Argument += POINTER_SIZE;

    return Result;
}

void PlatformWriteConsole(char *String)
{
#ifdef WINDOWS_OS
    OutputDebugStringA(String);
#else
    #error
#endif
}

uptr PlatformFormatString(char *Buffer, uptr BufferSize, char *Format, ...)
{
    uptr Result = 0;

    if(BufferSize < sizeof(char))
    {
        return Result;
    }

    va_args Argument = VABegin(&Format);
    char *Octal = "01234567";
    char *Decimal = "0123456789";
    char *HexadecimalLowercase = "0123456789abcdef";
    char *HexadecimalUppercase = "0123456789ABCDEF";
    for(char *c = Format; *c != '\0';)
    {
        if(*c != '%')
        {
            Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, *c);
            c++;
            continue;
        }

        if(*++c == '%')
        {
            Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, *c);            
            c++;
        }
        else if(*c == 'c')
        {
            int Value = VAGet(&Argument, int);
            Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, Value);            
            c++;
        }
        else if((*c == 'd') || (*c == 'i'))
        {
            s32 Value = VAGet(&Argument, s32);
            Result += CopySigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);            
            c++;
        }
        else if(*c == 'f')
        {
            f64 Value = VAGet(&Argument, f64);
            Result += CopyFloat64ToBuffer(Buffer + Result, BufferSize - Result, Value, 6);            
            c++;
        }
        else if((c[0] == 'l') && (c[1] == 'l') && ((c[2] == 'd') || (c[2] == 'i')))
        {
            s64 Value = VAGet(&Argument, s64);
            Result += CopySigned64ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c += 3;
        }
        else if((c[0] == 'l') && (c[1] == 'l') && (c[2] == 'u'))
        {
            u64 Value = VAGet(&Argument, u64);
            Result += CopyUnsigned64ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal); 
            c += 3;
        }
        else if(*c == 'o')
        {
            u32 Value = VAGet(&Argument, u32);
            Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 8, Octal);            
            c++;
        }
        else if(*c == 'p')
        {
            void *Value = VAGet(&Argument, void *);
            Result += CopyUnsigned64ToBuffer(Buffer + Result, BufferSize - Result, (u64)Value, 16, HexadecimalLowercase); 
            c++;
        }
        else if(*c == 's')
        {
            char *Value = VAGet(&Argument, char *);
            Result += CopyStringToBuffer(Buffer + Result, BufferSize - Result, Value, StringLength(Value), StringLength(Value)); 
            c++;
        }
        else if(*c == 'u')
        {
            u32 Value = VAGet(&Argument, u32);
            Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);            
            c++;
        }
        else if((*c == 'x') || (*c == 'X'))
        {
            u32 Value = VAGet(&Argument, u32);
            if(*c == 'x')
            {
                Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 16, HexadecimalLowercase);
            }
            else
            {
                Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 16, HexadecimalUppercase);
            }

            c++;
        }
        else
        {
            Assert(0);
            PlatformWriteConsole("Invalid specifier"); //@TODO:
            return Result;
        }
    }

    Argument = VAEnd();
    Buffer[Result] = '\0';

    return Result;
}

void *PlatformAllocateMemory(uptr Size)
{
    void *Result = 0;

    if(!Size)
    {
        Log("Size is %d", Size);
    }

#ifdef WINDOWS_OS
    Result = VirtualAlloc(0, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(!Result)
    {
        Log("VirtualAlloc() failed: 0x%X\n", GetLastError());
        return Result;
    }
#else
    #error
#endif

    return Result;
}

bool PlatformFreeMemory(void *Memory)
{
    bool Result = 0;

    if(!Memory)
    {
        Log("Memory is %p\n", Memory);
        return Result;
    }

#ifdef WINDOWS_OS
    if(!VirtualFree(Memory, 0, MEM_RELEASE))
    {
        Log("VirtualFree() failed: 0x%X\n", GetLastError());
    }
#else
    #error
#endif

    Result = 1;
    return Result;
}

bool PlatformReadFile(file *File, char *FileName)
{
    bool Result = 0;

#ifdef WINDOWS_OS
    HANDLE FileHandle = CreateFileA(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        Log("CreateFileA() failed: 0x%X\n", GetLastError());
        return Result;
    }

    LARGE_INTEGER FileSize = {0};
    if(!GetFileSizeEx(FileHandle, &FileSize))
    {
        Log("GetFileSizeEx() failed: 0x%X\n", GetLastError());
        CloseHandle(FileHandle);
        return Result;
    }

    File->Memory = PlatformAllocateMemory(FileSize.QuadPart);
    if(!File->Memory)
    {
        CloseHandle(FileHandle);
        return Result;
    }

    DWORD BytesRead = 0;
    if(!ReadFile(FileHandle, File->Memory, (DWORD)FileSize.QuadPart, &BytesRead, 0))
    {
        DWORD Error = GetLastError();
        if(Error != ERROR_IO_PENDING)
        {
            Log("ReadFile() failed: 0x%X\n", GetLastError());
            PlatformFreeMemory(File->Memory);
            File->Memory = 0;
            CloseHandle(FileHandle);
            return Result;
        }        
    }
    
    if(BytesRead != FileSize.QuadPart)
    {
        Log("ByteRead mismatch against file size\n");
        PlatformFreeMemory(File->Memory);
        File->Memory = 0;
        CloseHandle(FileHandle);
        return Result;
    }

    File->Size = FileSize.QuadPart;
    CloseHandle(FileHandle);
#else
    #error
#endif

    Result = 1;
    return Result;
}