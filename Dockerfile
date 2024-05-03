# Use a base image with your desired Linux distribution
FROM debian:bullseye

# Set environment variables for WeeChat
ENV WEECHAT_HOME /weechat

# Expose WeeChat port
# EXPOSE 6667

# Install WeeChat
RUN apt-get update && apt-get install -y weechat && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set up a non-root user to run WeeChat
RUN groupadd -r weechat && useradd -r -g weechat -d $WEECHAT_HOME -s /sbin/nologin -c "WeeChat user" weechat && \
    mkdir -p $WEECHAT_HOME && chown -R weechat:weechat $WEECHAT_HOME

# Set the working directory
WORKDIR $WEECHAT_HOME

# Switch to the non-root user
USER weechat

# Run WeeChat
CMD ["weechat"]
