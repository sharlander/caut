# this is a comment
the hash is not needed

but if you want to comment code you shoule put it there

# file { 'example':

file { 'example2':
  ensure => present,
  path   => '/tmp/examplefile',
  owner  => 'sth',
  group  => 'sth',
  mode   => '0775',
}

file { 'example3':
  ensure => directory,
  path   => '/tmp/exampledir',
  owner  => 'sth',
  group  => 'sth',
  mode   => '0775',
}

file { 'delte_example3':
  ensure => absent,
  path   => '/tmp/exampledir',
}

package { 'webserver':
  ensure => present,
}