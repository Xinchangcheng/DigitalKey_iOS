//
//  DeviceModel.h
//  DigitalKey
//
//  Created by SM2 on 2022/4/14.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface DKDeviceModel : NSObject

/* 设备 ID */
@property (copy, nonatomic) NSString *mobileId;

/* 手机名称 */
@property (copy, nonatomic) NSString *mobileName;

/* 手机品牌 */
@property (copy, nonatomic) NSString *mobileBrand;

/* 手机型号 */
@property (copy, nonatomic) NSString *mobileModel;

/* 操作系统版本 */
@property (copy, nonatomic) NSString *osVersion;

/* 设备绑定时间 */
@property (copy, nonatomic) NSString *bindTime;

- (id)initWithDictionary:(NSDictionary *)dic;

@end

NS_ASSUME_NONNULL_END
