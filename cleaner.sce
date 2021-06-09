// This file is released under the 3-clause BSD license. See COPYING-BSD.
// Generated by builder.sce : Please, do not edit this file
// cleaner.sce
// ------------------------------------------------------
curdir = pwd();
cleaner_path = get_file_path('cleaner.sce');
chdir(cleaner_path);
// ------------------------------------------------------
if fileinfo('loader.sce') <> [] then
  mdelete('loader.sce');
end
// ------------------------------------------------------
if fileinfo('Makelib') <> [] then
  unix_s('make -f Makelib clean');
  mdelete('Makelib');
end
// ------------------------------------------------------
if isdir('Debug') then
  rmdir('Debug','s');
end
// ------------------------------------------------------
if isdir('Release') then
  rmdir('Release','s');
end
// ------------------------------------------------------
if fileinfo('libTestBlock.dll') <> [] then
  mdelete('libTestBlock.dll');
end
// ------------------------------------------------------
chdir(curdir);
// ------------------------------------------------------
