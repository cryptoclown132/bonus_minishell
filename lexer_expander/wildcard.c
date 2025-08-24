#include "../minishell.h"

#include <sys/types.h>
#include <dirent.h>

// char  **read_curr_dir()
// {
//   const DIR *curr_dir = opendir(".");
//   struct dirent *next_dir;

//   // Error handling of opendir
//   next_dir = readdir(curr_dir);
//   while (next_dir)
//   {
//     next_dir = readdir(curr_dir);
//   }
// }

char  *wildcard(char *str)
{
	// Count entries in curr dic
	DIR     *curr_dir = opendir(".");
	ssize_t     entry_count = 0;
	while (readdir(curr_dir))
	++entry_count;
	closedir((DIR *)curr_dir);

	// Allocate enough memory
	char paths[entry_count][PATH_SIZE];

	// Populate the array with the actuall paths
	curr_dir = opendir(".");
	struct dirent *next_dir = readdir(curr_dir);
	ssize_t idx = 0;
	while (next_dir)
	{
		if (next_dir->d_name[0] != '.')
		{
			ft_strlcpy(paths[idx], next_dir->d_name, PATH_SIZE);
			idx++;
		}
		next_dir = readdir(curr_dir);
	}

  // Special case: str == "*"
if (!ft_strncmp(str, "*", ft_strlen(str)))
{
char  *tmp;
char  *ret = calloc(1, 1);

for (ssize_t i = 0; i < entry_count; i++)
{
	if (i != 0)
	{
	tmp = ret;
	ret = ft_strjoin(ret, " ");
	free(tmp);
	}
	tmp = ret;
	ret = ft_strjoin(ret, paths[i]);
	free(tmp);
}
free(str);
return (ret);
}

	// Split the string at every '*'
	char  **splits = ft_split(str, '*');
	if (!splits || !ft_strncmp(str, splits[0], ft_strlen(str)))
	return str;

	// Find all paths that match the pattern
	char  *matching_paths = ft_calloc(1, 1);
	for (ssize_t path_idx = 0; path_idx < entry_count; path_idx++)
	{
	char  *substr = paths[path_idx];
	for (ssize_t split_idx = 0; splits[split_idx]; split_idx++)
	{
		substr = ft_strnstr(substr, splits[split_idx], ft_strlen(substr));
		if (!substr)
		break;
	}

	if (substr)
	{
		char  *tmp = matching_paths;
		matching_paths = ft_strjoin(matching_paths, " ");
		free(tmp);
		tmp = matching_paths;
		matching_paths = ft_strjoin(matching_paths, paths[path_idx]);
		free(tmp);
	}
	}

	free(str);
	// Free splits
  return (matching_paths);
}
