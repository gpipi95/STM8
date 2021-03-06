
import os
import ycm_core
from clang_helpers import PrepareClangFlags

compilation_database_folder = ''

flags = [
'-Wall','-Wextra','-Werror',
'-std=c99',
'-x','c',
'-ISUB','C:\\Program Files (x86)\\COSMIC\\CXSTM8_EVAL\\Hstm8',
'-ISUB','C:\\Program Files (x86)\\STMicroelectronics\\st_toolset\\include',
'-I','.',
'-I','./core',
'-I','./testProj',
'-I','./DS18B20',
'-I','./TM1638',
'-I','./STM8S_StdPeriph_Driver/inc',
'-I','./PID'
]


if os.path.exists( compilation_database_folder ):
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None

SOURCE_EXTENSIONS = [ '.C', '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )


def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def IsHeaderFile( filename ):
  extension = os.path.splitext( filename )[ 1 ]
  return extension in [ '.H', '.h', '.hxx', '.hpp', '.hh' ]

def Subdirectories(directory):
  res = []
  for path, subdirs, files in os.walk(directory):
    for name in subdirs:
      item = os.path.join(path, name)
      res.append(item)
  return res

def IncludeFlagsOfSubdirectory( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_include_subdir = False
  path_flags = [ '-ISUB']
  for flag in flags:
    # include the directory of flag as well
    new_flag = [flag.replace('-ISUB', '-I')]

    if make_next_include_subdir:
      make_next_include_subdir = False
      for subdir in Subdirectories(os.path.join(working_directory, flag)):
        new_flag.append('-I')
        new_flag.append(subdir)

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_include_subdir = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        for subdir in Subdirectories(os.path.join(working_directory, path)):
            new_flag.append('-I' + subdir)
        break

    new_flags =new_flags + new_flag
  return new_flags

def GetCompilationInfoForFile( filename ):
  # The compilation_commands.json file generated by CMake does not have entries
  # for header files. So we do our best by asking the db for flags for a
  # corresponding source file, if any. If one exists, the flags for that file
  # should be good enough.
  if IsHeaderFile( filename ):
    basename = os.path.splitext( filename )[ 0 ]
    for extension in SOURCE_EXTENSIONS:
      replacement_file = basename + extension
      if os.path.exists( replacement_file ):
        compilation_info = database.GetCompilationInfoForFile(
          replacement_file )
        if compilation_info.compiler_flags_:
          return compilation_info
    return None
  return database.GetCompilationInfoForFile( filename )


def FlagsForFile( filename, **kwargs ):
#def FlagsForFile( filename ):
#  if database:
#    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
#    # python list, but a "list-like" StringVec object
#    compilation_info = database.GetCompilationInfoForFile( filename )
#    final_flags = PrepareClangFlags(
#        MakeRelativePathsInFlagsAbsolute(
#            compilation_info.compiler_flags_,
#            compilation_info.compiler_working_dir_ ),
#        filename )
#  else:
#    relative_to = DirectoryOfThisScript()
#    temp_flags = IncludeFlagsOfSubdirectory(flags, relative_to)
#    final_flags = MakeRelativePathsInFlagsAbsolute(temp_flags, relative_to )
  if database:
    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
    # python list, but a "list-like" StringVec object
    compilation_info = GetCompilationInfoForFile( filename )
    if not compilation_info:
      return None

    #final_flags = MakeRelativePathsInFlagsAbsolute(
    #  compilation_info.compiler_flags_,
    #  compilation_info.compiler_working_dir_ )
    final_flags = IncludeFlagsOfSubdirectory(
      compilation_info.compiler_flags_,
      compilation_info.compiler_working_dir_ )

  else:
    relative_to = DirectoryOfThisScript()
    #final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )
    final_flags = IncludeFlagsOfSubdirectory( flags, relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }

