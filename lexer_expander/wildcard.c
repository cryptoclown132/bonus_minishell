#include "../minishell.h"

#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>

bool   path_matches(char *pattern, char *path, bool wildcard_active)
{
  // test for '\0'
  if ((*pattern == '\0') && (*path == '\0'))
    return (true);
  if (*pattern == '\0' && *path != '\0')
    return (false);
  if (*pattern != '\0' && *path == '\0')
    return (false);

  if (*pattern == '*')
    return (path_matches(pattern + 1, path + 1, true));

  if (*pattern == *path && !wildcard_active)
    return(path_matches(pattern + 1, path + 1, false));
  if (*pattern == *path && wildcard_active)
  {
    if (path_matches(pattern + 1, path + 1, false) == false)
    {
      return (path_matches(pattern, path + 1, true));
    } 
    else
      return (true);
  }

  if (*pattern != *path && wildcard_active)
    return(path_matches(pattern, path + 1, true));
  if (*pattern != *path && !wildcard_active)
    return(false);
  
  printf("Error in wildcard\n");
  return (true);
}

void  append_path(char **matching_paths, char *to_append)
{
  char  *tmp;

  if (ft_strlen(*matching_paths) != 0)
  {
    tmp = *matching_paths;
    *matching_paths = ft_strjoin(*matching_paths, " ");
    free(tmp);
  }
  tmp = *matching_paths;
  *matching_paths = ft_strjoin(*matching_paths, to_append);
  free(tmp);
}

char  *wildcard(char *pattern)
{
  DIR           *dirp;
  struct dirent *dirent;
  char          *matching_paths;
  const bool    match_all = ft_strlen(pattern) == 1 && *pattern == '*';
  const bool    match_hidden_files = pattern[0] == '.';

  matching_paths = ft_calloc(1, 1);
  dirp = opendir(".");
  while ((dirent = readdir(dirp)))
  {
    if ((match_all && dirent->d_name[0] != '.')
      || (dirent->d_name[0] == '.' && match_hidden_files && path_matches(pattern, dirent->d_name, false))
      || path_matches(pattern, dirent->d_name, false))
      append_path(&matching_paths, dirent->d_name);
  }

  if (ft_strlen(matching_paths) == 0)
  {
    free(matching_paths);
    return (pattern);
  }
  return (matching_paths);
}
