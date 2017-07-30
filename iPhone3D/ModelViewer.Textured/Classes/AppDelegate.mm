#import "AppDelegate.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    m_window = [[UIWindow alloc] initWithFrame: screenBounds];
    m_view = [[GLView alloc] initWithFrame: screenBounds];
    
    UIViewController* viewController = [[UIViewController alloc] initWithNibName:nil bundle:nil];
    viewController.view = m_view;
    
    [m_window setRootViewController:viewController];
    [m_window makeKeyAndVisible];
    return YES;
}

- (void) dealloc
{
    [m_view release];
    [m_window release];
    [super dealloc];
}

@end
