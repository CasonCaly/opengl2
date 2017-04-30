# coding:utf-8
"""
SQlite3加密的python包装类
"""
import platform
import sys
from ctypes import c_int
from ctypes import CDLL
from ctypes import c_char_p
from ctypes import c_void_p
from ctypes import POINTER
from ctypes import c_double
from ctypes import c_ubyte
from ctypes import CFUNCTYPE
from ctypes import cast
from ctypes import pointer

class SQLite3Secure(object):
    """
    SQLite的加密类，所有SQLite能用的函数这边都能用
    """
    # 以下参数都是和打开数据库有关的
    OPEN_READONLY = 0x00000001
    OPEN_READWRITE = 0x00000002  # Ok for sqlite3_open_v2()
    OPEN_CREATE = 0x00000004  # Ok for sqlite3_open_v2()
    OPEN_DELETEONCLOSE = 0x00000008  # VFS only
    OPEN_EXCLUSIVE = 0x00000010  # VFS only
    OPEN_AUTOPROXY = 0x00000020  # VFS only
    OPEN_URI = 0x00000040  # Ok for sqlite3_open_v2()
    OPEN_MEMORY = 0x00000080  # Ok for sqlite3_open_v2()
    OPEN_MAIN_DB = 0x00000100  # VFS only
    OPEN_TEMP_DB = 0x00000200  # VFS only
    OPEN_TRANSIENT_DB = 0x00000400  # VFS only
    OPEN_MAIN_JOURNAL = 0x00000800  # VFS only
    OPEN_TEMP_JOURNAL = 0x00001000  # VFS only
    OPEN_SUBJOURNAL = 0x00002000  # VFS only
    OPEN_MASTER_JOURNAL = 0x00004000  # VFS only
    OPEN_NOMUTEX = 0x00008000  # Ok for sqlite3_open_v2()
    OPEN_FULLMUTEX = 0x00010000  # Ok for sqlite3_open_v2()
    OPEN_SHAREDCACHE = 0x00020000  # Ok for sqlite3_open_v2()
    OPEN_PRIVATECACHE = 0x00040000  # Ok for sqlite3_open_v2()
    OPEN_WAL = 0x00080000  # VFS only

    OK = 0 #Successful result
    ERROR = 1   #SQL error or missing database
    INTERNAL = 2   #Internal logic error in SQLite
    PERM = 3   #Access permission denied
    ABORT = 4   #Callback routine requested an abort
    BUSY = 5   #The database file is locked
    LOCKED = 6   #A table in the database is locked
    NOMEM = 7   #A malloc() failed
    READONLY = 8   #Attempt to write a readonly database
    INTERRUPT = 9   #Operation terminated by sqlite3_interrupt()*/
    IOERR = 10   #Some kind of disk I/O error occurred
    CORRUPT = 11   #The database disk image is malformed
    NOTFOUND = 12   #Unknown opcode in sqlite3_file_control()
    FULL = 13   #Insertion failed because database is full
    CANTOPEN = 14   #Unable to open the database file
    PROTOCOL = 15   #Database lock protocol error
    EMPTY = 16   #Database is empty
    SCHEMA = 17   #The database schema changed
    TOOBIG = 18   #String or BLOB exceeds size limit
    CONSTRAINT = 19   #Abort due to constraint violation
    MISMATCH = 20   #Data type mismatch
    MISUSE = 21   #Library used incorrectly
    NOLFS = 22   #Uses OS features not supported on host
    AUTH = 23   #Authorization denied
    FORMAT = 24   #Auxiliary database format error
    RANGE = 25   #2nd parameter to sqlite3_bind out of range
    NOTADB = 26   #File opened that is not a database file
    NOTICE = 27   #Notifications from sqlite3_log()
    WARNING = 28   #Warnings from sqlite3_log()
    ROW = 100  #sqlite3_step() has another row ready
    DONE = 101  #sqlite3_step() has finished executing

    lib = None
    is_lib_init = False
    sqlite3_exec_callback = CFUNCTYPE(c_void_p, c_int, POINTER(c_char_p), POINTER(c_char_p))

    @classmethod
    def init_library(cls):
        """
        动态库的初始化函数，只初始化一次
        """
        if SQLite3Secure.is_lib_init is True:
            return
        sysstr = platform.system()
        if sysstr == "Windows":
            SQLite3Secure.lib = CDLL("sqlite3secure.dll")
        elif sysstr == "Darwin":
            SQLite3Secure.lib = CDLL("sqlite3secure.dylib")
        else:
            return

        SQLite3Secure.is_lib_init = True

        # int sqlite3_open(const char *filename, sqlite3 **ppDb)
        SQLite3Secure.lib.sqlite3_open.argtypes = [c_char_p, POINTER(c_void_p)]
        SQLite3Secure.lib.sqlite3_open.restype = c_int

        # int sqlite3_open16(const void *filename, sqlite3 **ppDb)
        SQLite3Secure.lib.sqlite3_open16.argtypes = [c_void_p, POINTER(c_void_p)]
        SQLite3Secure.lib.sqlite3_open16.restype = c_int

        # int sqlite3_open_v2(const void *filename, sqlite3 **ppDb, int flags,  const char *zVfs)
        SQLite3Secure.lib.sqlite3_open_v2.argtypes = [c_char_p, POINTER(c_void_p), c_int, c_char_p]
        SQLite3Secure.lib.sqlite3_open_v2.restype = c_int

        # int sqlite3_close(sqlite3* db)
        SQLite3Secure.lib.sqlite3_close.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_close.restype = c_int

        # int sqlite3_close_v2(sqlite3* db)
        SQLite3Secure.lib.sqlite3_close_v2.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_close_v2.restype = c_int

        # int sqlite3_prepare_v2(sqlite3* db, const char *zSql,  int nByte, sqlite3_stmt **ppStmt, const char **pzTail)
        SQLite3Secure.lib.sqlite3_prepare_v2.argtypes = [c_void_p, c_char_p, c_int, POINTER(c_void_p), POINTER(c_char_p)]
        SQLite3Secure.lib.sqlite3_prepare_v2.restype = c_int

        # int sqlite3_prepare16(sqlite3* db, const void *zSql,  int nByte, sqlite3_stmt **ppStmt, const void **pzTail)
        SQLite3Secure.lib.sqlite3_prepare16.argtypes = [c_void_p, c_void_p, c_int, POINTER(c_void_p), POINTER(c_void_p)]
        SQLite3Secure.lib.sqlite3_prepare16.restype = c_int

        # int sqlite3_prepare16_v2(sqlite3* db, const void *zSql,  int nByte, sqlite3_stmt **ppStmt, const void **pzTail)
        SQLite3Secure.lib.sqlite3_prepare16_v2.argtypes = [c_void_p, c_void_p, c_int, POINTER(c_void_p), POINTER(c_void_p)]
        SQLite3Secure.lib.sqlite3_prepare16_v2.restype = c_int

        # int sqlite3_step(sqlite3_stmt*)
        SQLite3Secure.lib.sqlite3_step.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_step.restype = c_int

        # int sqlite3_reset(sqlite3_stmt*)
        SQLite3Secure.lib.sqlite3_reset.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_reset.restype = c_int

        # int sqlite3_column_count(sqlite3_stmt*)
        SQLite3Secure.lib.sqlite3_column_count.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_column_count.restype = c_int

        # int sqlite3_column_int(sqlite3_stmt*, int iCol)
        SQLite3Secure.lib.sqlite3_column_int.argtypes = [c_void_p, c_int]
        SQLite3Secure.lib.sqlite3_column_int.restype = c_int

        # double sqlite3_column_double(sqlite3_stmt*, int iCol)
        SQLite3Secure.lib.sqlite3_column_double.argtypes = [c_void_p, c_int]
        SQLite3Secure.lib.sqlite3_column_double.restype = c_double

        # const unsigned char * sqlite3_column_text(sqlite3_stmt*, int iCol)
        SQLite3Secure.lib.sqlite3_column_text.argtypes = [c_void_p, c_int]
        SQLite3Secure.lib.sqlite3_column_text.restype = POINTER(c_ubyte)

        # int sqlite3_exec(sqlite3*, const char *sql, int (*callback)(void*,int,char**,char**), void *, char **errmsg)
        SQLite3Secure.lib.sqlite3_exec.argtypes = [c_void_p, c_char_p, SQLite3Secure.sqlite3_exec_callback, c_void_p, POINTER(c_char_p)]
        SQLite3Secure.lib.sqlite3_exec.restype = c_int

        # int sqlite3_step(sqlite3_stmt*)
        SQLite3Secure.lib.sqlite3_step.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_step.restype = c_int

        # int sqlite3_finalize(sqlite3_stmt*)
        SQLite3Secure.lib.sqlite3_finalize.argtypes = [c_void_p]
        SQLite3Secure.lib.sqlite3_finalize.restype = c_int

        # int sqlite3_finalize(sqlite3_stmt*, const void *pKey, int nKey)
        SQLite3Secure.lib.sqlite3_key.argtypes = [c_void_p, c_void_p, c_int]
        SQLite3Secure.lib.sqlite3_key.restype = c_int

    def __init__(self):
        SQLite3Secure.init_library()
        self.sqlite3 = c_void_p()
        self.sqlite3_stmt = c_void_p()

    def open_v2(self, file_name, flags):
        """
        对应sqlite3_open_v2函数
        file_name 为sqlite路径
        flags 来自静态变量OPEN_XXX系列
        """
        zvfs = c_char_p()
        return SQLite3Secure.lib.sqlite3_open_v2(file_name, pointer(self.sqlite3), flags, zvfs)

    def prepare_v2(self, sql):
        """
        对应sqlite3_prepare_v2函数
        @param sql 为标准的sql语句
        @return int, string  返回为两个值，一个执行结果，一个是包含错误的文本，如果是第一个结果等于OK的话，那么表示成功
        """
        tail = c_char_p()
        ret = SQLite3Secure.lib.sqlite3_prepare_v2(self.sqlite3, sql, len(sql), \
                                                pointer(self.sqlite3_stmt), pointer(tail))
        return ret, tail

    def step(self):
        """
        对应sqlite3_step函数
        """
        ret = SQLite3Secure.lib.sqlite3_step(self.sqlite3_stmt)
        return ret

    def reset(self):
        """
        对应sqlite3_reset函数
        """
        ret = SQLite3Secure.lib.sqlite3_reset(self.sqlite3_stmt)
        return ret

    def column_text(self, index):
        """
        对应sqlite3_column_text函数
        index 用于表示第几列
        """
        c_ubyte_pointer_value = SQLite3Secure.lib.sqlite3_column_text(self.sqlite3_stmt, index)
        c_char_value = cast(c_ubyte_pointer_value, c_char_p)
        return c_char_value.value

    def key(self, key):
        """
        对应sqlite3_key函数,设置解密用的key
        key 解密用的key
        """
        c_key = cast(key, c_void_p)
        return SQLite3Secure.lib.sqlite3_key(self.sqlite3, c_key, len(key))


def main():
    """
    测试函数
    """
    sqlite = SQLite3Secure()
    ret = sqlite.open_v2("lang.db", SQLite3Secure.OPEN_READONLY)
    if ret != SQLite3Secure.OK:
        return

    ret = sqlite.key("aea047b0eacef48847ca7b1cadfc98a3")
    if ret != SQLite3Secure.OK:
        return
    ret, tail = sqlite.prepare_v2("select * from lang")
    if ret != SQLite3Secure.OK:
        print tail
        return

    code_type = sys.getfilesystemencoding()
    while True:
        ret = sqlite.step()
        if ret != SQLite3Secure.ROW:
            break
        key = sqlite.column_text(0)
        value = sqlite.column_text(1)
        print value.decode('UTF-8').encode(code_type)
if __name__ == "__main__":
    main()

