<?php

function kraftwagen_archive_prepare_location($name, $path) {
  $root = kraftwagen_context_get_option('root-path');

  $dir_archive = kraftwagen_context_get_option('archive-dir');

  if (!$dir_archive) {
    return drush_set_error(dt('Could not archive !name from !path, because we could find the archive directory.', array('!name' => $name, '!path' => $path)));
  }

  if (!is_dir($root . DIRECTORY_SEPARATOR . $dir_archive)) {
    if (!is_writable($root)) {
      return drush_set_error(dt('Could not archive !name from !path, because the archive directory !archive is not writable.', array('!name' => $name, '!path' => $path, '!archive' => $root)));
    }

    if (!drush_mkdir($root . DIRECTORY_SEPARATOR . $dir_archive)) {
      return drush_set_error(dt('Could not archive !name from !path, because the directory !dir could not be created.', array('!name' => $name, '!path' => $path, '!dir' => $root . DIRECTORY_SEPARATOR . $dir_archive)));
    }

    // Notice, usually not visible.
    drush_log(dt('Directory !dir created to archive !name.', array('!dir' => $root . DIRECTORY_SEPARATOR . $dir_archive, '!name' => $name)));
  }

  if (!is_dir($root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name)) {
    if (!is_writable($root . DIRECTORY_SEPARATOR . $dir_archive)) {
      return drush_set_error(dt('Could not archive !name from !path, because the archive directory !archive is not writable.', array('!name' => $name, '!path' => $path, '!archive' => $root . DIRECTORY_SEPARATOR . $dir_archive)));
    }

    if (!drush_mkdir($root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name)) {
      return drush_set_error(dt('Could not archive !name from !path, because the directory !dir could not be created.', array('!name' => $name, '!path' => $path, '!dir' => $root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name)));
    }

    // Notice, usually not visible.
    drush_log(dt('Directory !dir created to archive !name.', array('!dir' => $root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name, '!name' => $name)));
  }

  if (!is_writable($root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name)) {
    return drush_set_error(dt('Could not archive !name from !path, because the archive directory !archive is not writable.', array('!name' => $name, '!path' => $path, '!archive' => $root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name)));
  }

  if (!($date_pattern = kraftwagen_context_get_option('date-pattern'))) {
    return drush_set_error(dt('Could not archive !name from !path, because we have no date pattern set', array('!name' => $name, '!path' => $path)));
  }

  $date = date($date_pattern);

  $suffix = 0;
  do {
    $target = $root . DIRECTORY_SEPARATOR . $dir_archive . DIRECTORY_SEPARATOR . $name . DIRECTORY_SEPARATOR . $date . ($suffix ? "-{$suffix}" : '') . '-' . $name;
    $suffix += 1;
  } while (@lstat($target));

  return $target;
}

function kraftwagen_archive_file($name, $path) {
  if (!($target = kraftwagen_archive_prepare_location($name, $path))) {
    return $target;
  }

  if (drush_op('copy', $path, $target) !== TRUE) {
    return drush_set_error(dt('Could not archive !name from !path to !target', array('!name' => $name, '!path' => $path, '!target' => $target)));
  }

  return $target;
}

function kraftwagen_archive_directory($name, $path) {
  if (!($target = kraftwagen_archive_prepare_location($name, $path))) {
    return $target;
  }

  if (drush_op('drush_copy_dir', $path, $target) !== TRUE) {
    return drush_set_error(dt('Could not archive !name from !path to !target', array('!name' => $name, '!path' => $path, '!target' => $target)));
  }

  return $target;
}

