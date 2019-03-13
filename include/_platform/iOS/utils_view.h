//
//  utils_view.h
//  implement
//
//  Created by brainpoint on 6/16/15.
//  Copyright (c) 2015 citong. All rights reserved.
//

#ifndef implement_ctUtilsView_hpp
#define implement_ctUtilsView_hpp

namespace hyc {
    
    /**
     * 获得当前显示的vc.
     */
    inline UIViewController* utils_view_getCurViewController();
    
    /**
     * 获得指定View的ViewController.
     */
    inline UIViewController* utils_view_getViewController( UIView* view );
    
    /**
     * 将view转成UIImage对象.
     */
    inline UIImage*          utils_view_to_uiimage( UIView* view, CGRect rect );
    
    
    
    
} // namespace hyc.





namespace hyc {

    inline UIViewController* utils_view_getCurViewController()
    {
        UIViewController *result = nil;
        
        UIWindow * window = [[UIApplication sharedApplication] keyWindow];
        if (window.windowLevel != UIWindowLevelNormal)
        {
            NSArray *windows = [[UIApplication sharedApplication] windows];
            for(UIWindow * tmpWin in windows)
            {
                if (tmpWin.windowLevel == UIWindowLevelNormal)
                {
                    window = tmpWin;
                    break;
                }
            }
        }
        
        UIView *frontView = [[window subviews] objectAtIndex:0];
        id nextResponder = [frontView nextResponder];
        
        if ([nextResponder isKindOfClass:[UIViewController class]])
            result = nextResponder;
        else
            result = window.rootViewController;
        
        if (result && [result isKindOfClass:[UINavigationController class]])
        {
            return ((UINavigationController*)result).visibleViewController;
        }
        
        return result;
    }
    
    inline UIViewController* utils_view_getViewController( UIView* view )
    {
        if (!view)  return nil;
        
        for (UIView* next = [view superview]; next; next = next.superview) {
            UIResponder *nextResponder = [next nextResponder];
            if ([nextResponder isKindOfClass:[UIViewController class]]) {
                return (UIViewController *)nextResponder;
            }
        }
        return nil;
    }
    
    
    inline UIImage* utils_view_to_uiimage( UIView* view, CGRect rect )
    {
        UIGraphicsBeginImageContext(view.bounds.size);
        [view.layer renderInContext:UIGraphicsGetCurrentContext()];
        UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        
        UIGraphicsBeginImageContext(rect.size);
        rect.origin.x = - rect.origin.x;
        rect.origin.y = - rect.origin.y;
        [image drawAtPoint:rect.origin];
        image = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        
        return image;
    }
    
}

#endif

