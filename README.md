# C++ File Downloader
c++ file downloader is a cli tool for downloading a specified list of files into a specified folder through HTTP/HTTPS.
## Requirements
boost - 1.91.0\
openssl - 3.6.2
## Usage
```sh
cppfiledownloader [URL LIST] [TARGET DIRECTORY] [MAX CONCURRENT DOWNLOADS]
```
[URL LIST] - a text file containing a list of URLs to download. URLs must be written one per line.\
[TARGET DIRECTORY] - directory, where downloaded files will be placed.\
[MAX CONCURRENT DOWNLOADS] - maximum number of concurrent downloads.