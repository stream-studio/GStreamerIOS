#
# Be sure to run `pod lib lint GStreamerIOS.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'GStreamerIOS'
  s.version          = '0.1.0'
  s.summary          = 'GStreamerIOS'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
GStreamer IOS Framework.
                       DESC

  s.homepage         = 'https://stream.studio'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { '828542' => 'ludovic.bouguerra@stream.studio' }
  s.source           = { :git => 'https://github.com/stream-studio/GStreamerIOS.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.ios.deployment_target = '14.0'
  
  s.ios.source_files = 'GStreamerIOS/Classes/**/*',  'GStreamerIOS/Headers/**/*'

  s.xcconfig = { 'HEADER_SEARCH_PATHS' => '"/Library/Developer/GStreamer/iPhone.sdk/GStreamer.framework/Headers"',  'FRAMEWORK_SEARCH_PATHS' => '"/Library/Developer/GStreamer/iPhone.sdk"'}



  s.ios.resource_bundles = {
     'GStreamerIOS' => ['GStreamerIOS/Assets/**/*']
   }

  s.ios.libraries = 'resolv', 'iconv', 'stdc++'
  s.ios.frameworks = 'AssetsLibrary', 'VideoToolbox', 'Avfoundation', 'CoreMedia', 'GStreamer', 'AudioToolbox', 'CoreAudio', 'CoreGraphics'
  # s.dependency 'AFNetworking', '~> 2.3'

end
