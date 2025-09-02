ami@amiden:~/work/repos/OrcaSlicer$ git diff
diff --git a/Dockerfile b/Dockerfile
index 0cc12ac2a..8961d0167 100644
--- a/Dockerfile
+++ b/Dockerfile
@@ -54,6 +54,8 @@ RUN apt-get update && apt-get install  -y \
 ENV LC_ALL=en_US.utf8
 RUN locale-gen $LC_ALL

+RUN userdel -r ubuntu || true && groupdel ubuntu || true
+
 # Set this so that Orca Slicer doesn't complain about
 # the CA cert path on every startup
 ENV SSL_CERT_FILE=/etc/ssl/certs/ca-certificates.crt
