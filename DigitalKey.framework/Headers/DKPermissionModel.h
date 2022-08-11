//
//  DKPermissionModel.h
//  DigitalKey
//
//  Created by SM2 on 2022/4/8.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface DKPermissionModel : NSObject <NSCoding>

/// 权限标识
@property (copy, nonatomic) NSString *permissionMask;

/// 权限分类描述
@property (copy, nonatomic) NSString *permissionDescription;

/// 权限类型 1 近控，2 远控
@property (copy, nonatomic) NSString *permissionType;

/// 是否选择
@property (assign, nonatomic) BOOL isChoice;

- (id)initWithDictionary:(NSDictionary *)dic;



@end

NS_ASSUME_NONNULL_END
