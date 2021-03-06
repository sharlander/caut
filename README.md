caut:
=====

A puppet like c automation tool
At the moment 'caut' only supports files

---

Overview:
---------

- Usage
- Manifests
- Templates
- Facts
- Facter
- Options
- Installation

---

Usage:
------

'caut' can be called without options and arguments set, but for more information have a look at the options section

<pre>
caut [OPTIONS] [ARGUMENTS]
</pre>

---

Manifests:
----------

The manifests syntax should be well known from puppet

#### Example manifest

<pre>

file { 'example':
  ensure => present,
  path   => '/srv/example_file',
  owner  => 'root',
  group  => 'root',
  mode   => '0664',
}

</pre>

---

Templates:
----------

As you see above you don't need a source or content parameter
'caut' dont difference between static files and templates
all source files are stored with the "virtual" name in the templates folder

#### Example template

<pre>

This is an example content

my computers hostname is: <%$hostname%>
my ip is: <%$ipaddress%>

</pre>

---

Facts:
------

The variables used in templates are stored in seperate files called fact files
those contain only "fact"="value" statements,
comments are not supported but the last occurence of a fact is used so just copy a fact below another to override the first

#### Example facts

<pre>

hostname=mypc.domain.local
ipaddress=10.10.10.2

</pre>

---

#### Facter

'caut' also supports facter, but it is not integrated
as facter is very slow, there is only an option to import all facter facts into the facts folder

Those facts will be stored in facts/common.ft

Note: 'caut' will first look through the common.ft file and then the facts-file, so fact-files override values form common.ft

---

Options:
--------

#### -f, --facter
Import facts from the facter program

#### -p, --path "argument"
Set the module path (default is pwd)

#### -d, --debug
Show debug messages

#### -v, --verbose
Set verbose mode

#### --version
Show program version

#### -h, --help
Display the command line help

---

Installation:
-------------

The simplest way to compile this package is:

1. `cd' to the directory containing the package's source code and type
   `./configure' to check your system for dependencies

2. Type `make' to compile the package.

3. Type `make install' to install the programs and any data files and
   documentation.  When installing into a prefix owned by root, it is
   recommended that the package be configured and built as a regular
   user, and only the `make install' phase executed with root
   privileges.
