function funct_nginxaddvhost {

echo ""
cecho "---------------------------------------------" $boldyellow

read -ep "Enter vhost domain name you want to add (without www. prefix): " vhostname

echo ""

if [ ! -d /home/nginx/domains/$vhostname ]; then

# Checking Permissions, making directories, example index.html
mkdir -p /home/nginx/domains/$vhostname/{public,private,log,backup}

cat > "/home/nginx/domains/$vhostname/public/index.html" <<END
<html>
<head>
<title>$vhostname</title>
</head>
<body>
<p>Welcome to $vhostname. This index.html page can be removed.</p>

<p>Useful Centmin Mod info and links to bookmark.</p>
</body>
</html>
END

    cp -R $CUR_DIR/htdocs/custom_errorpages/* /home/nginx/domains/$vhostname/public

chown -R nginx:nginx "/home/nginx/domains/$vhostname"
find "/home/nginx/domains/$vhostname" -type d -exec chmod g+s {} \;

# Setting up Nginx mapping
cat > "/usr/local/nginx/conf/conf.d/$vhostname.conf" <<END
# redirect from non-www to www 
# uncomment, save file and restart Nginx to enable
#server {
#            listen   80;
#            server_name $vhostname;
#            return 301 \$scheme://www.${vhostname}\$request_uri;
#       }

server {
	listen [::]:80;
	listen   80;
	server_name $vhostname www.$vhostname;

# ngx_pagespeed & ngx_pagespeed handler
#include /usr/local/nginx/conf/pagespeed.conf;
#include /usr/local/nginx/conf/pagespeedhandler.conf;
#include /usr/local/nginx/conf/pagespeedstatslog.conf;

  # limit_conn limit_per_ip 16;
  # ssi  on;

  access_log /home/nginx/domains/$vhostname/log/access.log combined buffer=32k;
  error_log /home/nginx/domains/$vhostname/log/error.log;

  root /home/nginx/domains/$vhostname/public;
set \$cache_uri \$request_uri;

  # POST requests and urls with a query string should always go to PHP
  if (\$request_method = POST) {
    set \$cache_uri 'null cache';
  }   
  if (\$query_string != "") {
    set \$cache_uri 'null cache';
  }   

  # Don't cache uris containing the following segments
  if (\$request_uri ~* "(/wp-admin/|/xmlrpc.php|/wp-(app|cron|login|register|mail).php|wp-.*.php|/feed/|index.php|wp-comments-popup.php|wp-links-opml.php|wp-locations.php|sitemap(_index)?.xml|[a-z0-9_-]+-sitemap([0-9]+)?.xml)") {
    set \$cache_uri 'null cache';
  }   

  # Don't use the cache for logged in users or recent commenters
  if (\$http_cookie ~* "comment_author|wordpress_[a-f0-9]+|wp-postpass|wordpress_logged_in") {
    set \$cache_uri 'null cache';
  }
  location / {

# block common exploits, sql injections etc
#include /usr/local/nginx/conf/block.conf;

  # Enables directory listings when index file not found
  #autoindex  on;

  # Shows file listing times as local time
  #autoindex_localtime on;

  # Enable for vBulletin usage WITHOUT vbSEO installed
  #try_files		\$uri \$uri/ /index.php;
	try_files /wp-content/cache/supercache/\$http_host/\$cache_uri/index.html \$uri \$uri/ /index.php ;
  }

  include /usr/local/nginx/conf/staticfiles.conf;
  include /usr/local/nginx/conf/php.conf;
  include /usr/local/nginx/conf/drop.conf;
  #include /usr/local/nginx/conf/errorpage.conf;
}
END

echo 
cecho "---------------------------------------------" $boldyellow
service nginx reload

echo 
cecho "---------------------------------------------" $boldyellow
cecho "vhost for $vhostname created successfully" $boldwhite
cecho "vhost conf file for $vhostname created: /usr/local/nginx/conf/conf.d/$vhostname.conf" $boldwhite
cecho "upload files to /home/nginx/domains/$vhostname/public" $boldwhite
cecho "vhost log files directory is /home/nginx/domains/$vhostname/log" $boldwhite

echo
cecho "Current vhost listing at: /usr/local/nginx/conf/conf.d/" $boldwhite

ls -Alhrt /usr/local/nginx/conf/conf.d/ | awk '{ printf "%-4s%-4s%-8s%-6s %s\n", $6, $7, $8, $5, $9 }'

cecho "---------------------------------------------" $boldyellow

else

echo ""
cecho "---------------------------------------------" $boldyellow
cecho "vhost for $vhostname already exists" $boldwhite
cecho "/home/nginx/domains/$vhostname already exists" $boldwhite
cecho "---------------------------------------------" $boldyellow
echo ""

fi


}