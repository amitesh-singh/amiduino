# fixing sound on laptop

```
vim ~/.config/pipewire/pipewire.conf.d/50-fix-format.conf

context.objects = [
  {
    factory = adapter
    args = {
      factory.name           = api.alsa.pcm.sink
      node.name              = "alsa-speaker"
      node.description       = "Meteor Lake Speaker"
      media.class            = Audio/Sink
      api.alsa.path          = "hw:2,2"
      audio.format           = S16LE
      audio.rate             = 48000
      audio.channels         = 2
      audio.position         = [ FL FR ]
      priority.session       = 1010
      priority.driver        = 1010
    }
  }
]


change api.alsa.path = "hw:1,2" or "hw:2,2" depending upon aplay -l

restart the services afterwards


systemctl --user restart pipewire pipewire-pulse wireplumber


```
