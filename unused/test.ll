; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.TestStruct = type { i8, i32, float, i8*, i32, double }

@switch.table = private unnamed_addr constant [8 x i32] [i32 4, i32 5, i32 0, i32 0, i32 0, i32 1, i32 2, i32 3]

; Function Attrs: nounwind uwtable
define i32 @foo(i32) local_unnamed_addr #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca [2 x float], align 4
  %5 = alloca i64, align 8
  %6 = alloca [6 x %struct.TestStruct], align 16
  store volatile i32 %0, i32* %2, align 4
  %7 = bitcast i32* %3 to i8*
  call void @llvm.lifetime.start(i64 4, i8* %7)
  store volatile i32 10, i32* %3, align 4
  %8 = load volatile i32, i32* %3, align 4
  %9 = bitcast [2 x float]* %4 to i8*
  call void @llvm.lifetime.start(i64 8, i8* %9) #2
  %10 = getelementptr inbounds [2 x float], [2 x float]* %4, i64 0, i64 0
  store float 1.000000e+01, float* %10, align 4, !tbaa !1
  %11 = getelementptr inbounds [2 x float], [2 x float]* %4, i64 0, i64 1
  store float 1.000000e+01, float* %11, align 4, !tbaa !1
  %12 = bitcast i64* %5 to i8*
  call void @llvm.lifetime.start(i64 8, i8* %12) #2
  store i64 4294967280, i64* %5, align 8, !tbaa !5
  %13 = getelementptr inbounds [6 x %struct.TestStruct], [6 x %struct.TestStruct]* %6, i64 0, i64 0, i32 0
  call void @llvm.lifetime.start(i64 240, i8* %13) #2
  br label %14

; <label>:14:                                     ; preds = %14, %1
  %15 = phi i64 [ 0, %1 ], [ %29, %14 ]
  %16 = phi i32 [ undef, %1 ], [ %28, %14 ]
  %17 = trunc i64 %15 to i32
  %18 = srem i32 %17, 2
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds [2 x float], [2 x float]* %4, i64 0, i64 %19
  %21 = bitcast float* %20 to i8*
  %22 = getelementptr inbounds [6 x %struct.TestStruct], [6 x %struct.TestStruct]* %6, i64 0, i64 %15, i32 3
  store volatile i8* %21, i8** %22, align 8, !tbaa !7
  %23 = load volatile i32, i32* %2, align 4
  %24 = trunc i64 %15 to i32
  %25 = add i32 %23, %24
  %26 = getelementptr inbounds [6 x %struct.TestStruct], [6 x %struct.TestStruct]* %6, i64 0, i64 %15, i32 1
  store volatile i32 %25, i32* %26, align 4, !tbaa !12
  %27 = load volatile i32, i32* %26, align 4, !tbaa !12
  %28 = add nsw i32 %27, %16
  %29 = add nuw nsw i64 %15, 1
  %30 = icmp eq i64 %29, 6
  br i1 %30, label %31, label %14

; <label>:31:                                     ; preds = %14
  %32 = shl i32 %8, 2
  %33 = add i32 %28, %32
  %34 = load volatile i32, i32* %2, align 4
  %35 = icmp ugt i32 %34, 5
  %36 = load float, float* %11, align 4
  %37 = fcmp ogt float %36, 5.000000e+00
  %38 = and i1 %35, %37
  %39 = load volatile i32, i32* %3, align 4
  br i1 %38, label %40, label %47

; <label>:40:                                     ; preds = %31
  %41 = add i32 %39, 5
  store volatile i32 %41, i32* %3, align 4
  %42 = add i32 %33, 10
  %43 = load volatile i32, i32* %2, align 4
  %44 = and i32 %43, 3
  %45 = zext i32 %44 to i64
  %46 = getelementptr inbounds [6 x %struct.TestStruct], [6 x %struct.TestStruct]* %6, i64 0, i64 %45, i32 3
  store volatile i8* %12, i8** %46, align 8, !tbaa !7
  br label %50

; <label>:47:                                     ; preds = %31
  %48 = add i32 %39, 50
  store volatile i32 %48, i32* %3, align 4
  %49 = add i32 %33, 100
  br label %50

; <label>:50:                                     ; preds = %47, %40
  %51 = phi i32 [ %42, %40 ], [ %49, %47 ]
  %52 = load volatile i32, i32* %3, align 4
  switch i32 %52, label %58 [
    i32 10, label %53
    i32 15, label %56
  ]

; <label>:53:                                     ; preds = %50
  %54 = load volatile i32, i32* %3, align 4
  %55 = shl i32 %54, 1
  store volatile i32 %55, i32* %3, align 4
  br label %66

; <label>:56:                                     ; preds = %50
  %57 = shl i32 %51, 1
  br label %66

; <label>:58:                                     ; preds = %50
  %59 = load volatile i32, i32* %2, align 4
  %60 = and i32 %59, 3
  %61 = zext i32 %60 to i64
  %62 = getelementptr inbounds [6 x %struct.TestStruct], [6 x %struct.TestStruct]* %6, i64 0, i64 %61, i32 3
  %63 = load volatile i8*, i8** %62, align 8, !tbaa !7
  %64 = ptrtoint i8* %63 to i64
  %65 = trunc i64 %64 to i32
  br label %66

; <label>:66:                                     ; preds = %58, %56, %53
  %67 = phi i32 [ %65, %58 ], [ %57, %56 ], [ %51, %53 ]
  %68 = load volatile i32, i32* %3, align 4
  %69 = icmp ugt i32 %68, 50
  br i1 %69, label %70, label %72

; <label>:70:                                     ; preds = %66
  %71 = load volatile i32, i32* %3, align 4
  br label %75

; <label>:72:                                     ; preds = %66
  %73 = load volatile i32, i32* %2, align 4
  %74 = add i32 %73, %67
  br label %75

; <label>:75:                                     ; preds = %72, %70
  %76 = phi i32 [ %71, %70 ], [ %74, %72 ]
  %77 = load volatile i32, i32* %2, align 4
  %78 = add i32 %77, %76
  %79 = urem i32 %78, 6
  %80 = zext i32 %79 to i64
  %81 = xor i64 %80, 4
  %82 = getelementptr inbounds [8 x i32], [8 x i32]* @switch.table, i64 0, i64 %81
  %83 = load i32, i32* %82, align 4
  call void @llvm.lifetime.end(i64 240, i8* nonnull %13) #2
  call void @llvm.lifetime.end(i64 8, i8* %12) #2
  call void @llvm.lifetime.end(i64 8, i8* nonnull %9) #2
  call void @llvm.lifetime.end(i64 4, i8* nonnull %7)
  ret i32 %83
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.1 (branches/release_39 291077)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"float", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !3, i64 0}
!7 = !{!8, !10, i64 16}
!8 = !{!"", !3, i64 0, !9, i64 4, !2, i64 8, !10, i64 16, !9, i64 24, !11, i64 32}
!9 = !{!"int", !3, i64 0}
!10 = !{!"any pointer", !3, i64 0}
!11 = !{!"double", !3, i64 0}
!12 = !{!8, !9, i64 4}
