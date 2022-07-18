//
//  DKInfoModel.h
//  DigitalKey
//
//  Created by SM2 on 2022/4/12.
//

#import <Foundation/Foundation.h>
#import <DigitalKey/DKPermissionModel.h>

NS_ASSUME_NONNULL_BEGIN

/* 蓝牙钥匙状态
 * DigitalKeyDownloaded 状态，仅用于被分享者，当本地无钥匙 & isDownload = yes 为该状态。
 */
typedef NS_ENUM (NSUInteger, DigitalKeySatus) {
    DigitalKeyUnknown = 0,           // 未知
    DigitalKeyNormal  = 1,           // 正常  （钥匙正常，并本地存在）
    DigitalKeyFrozen  = 2,           // 冻结
    DigitalKeyRevoke  = 3,           // 吊销
    DigitalKeyExpired = 4,           // 过期
    DigitalKeyNonActivated = 5,      // 未激活 (仅车主存在未激活状态)
    DigitalKeyNonDownload  = 6,      // 未下载
    DigitalKeyDownloaded   = 10,     // 已下载（仅非车主，钥匙正常，本地不存在）
};

typedef NS_ENUM (NSUInteger, DigitalKeyChangeStatus) {
    DigitalKeyChangeNormal   = 0,     // 正常
    DigitalKeyChangeDelete   = 1,     // 司机用户删除
    DigitalKeyChangeCarOwner = 2,     // 车辆变更车主
};
@interface DKInfoModel : NSObject

/// 数字钥匙 ID
@property (copy, nonatomic) NSString *dkID;

// 手机 ID
@property (copy, nonatomic) NSString *mobileId;

// 分享者手机号
@property (copy, nonatomic) NSString *phoneNumber;

/// 数字钥匙生效时间
@property (copy, nonatomic) NSString *startTime;

/// 数字钥匙到期时间
@property (copy, nonatomic) NSString *endTime;

/// 激活钥匙请求时间或钥匙分享请求时间
@property (copy, nonatomic) NSString *requestTime;

/// 数字钥匙类型 0: 车主身份 1: 亲友身份 2: 临时身份 3: 试乘试驾 4: 租赁类型
@property (assign, nonatomic) int keyType;

/// 权限信息
@property (strong, nonatomic) NSArray<DKPermissionModel *> *permissionList;

/// 数字钥匙可用次数
@property (assign, nonatomic) int times;

/// 是否已下载 0: 未下载 1: 已下载
@property (assign, nonatomic) int isDownload;

/// Vin
@property (copy, nonatomic) NSString *vin;

/// UserID
@property (copy, nonatomic) NSString *userId;

/// 数字钥匙状态
@property (assign, nonatomic) DigitalKeySatus status;

/// 数字钥匙状态变更原因 status == DigitalKeyRevoke this field have value. 
@property (assign, nonatomic) DigitalKeyChangeStatus changeStatus;

- (id)initWithDictionary:(NSDictionary *)dic;

- (id)initWithObject:(id)obj;

@end

NS_ASSUME_NONNULL_END
