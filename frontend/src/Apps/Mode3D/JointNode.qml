import QtQuick
import QtQuick3D

Model {
    // угол поворота модели
    property real angle: jointAngle.value * gearRatio

    // коэффициент редуктора
    property real gearRatio: 1

    // зависимый joint, угол которого меняется при сдвиге мышкой
    property JointAngle jointAngle: JointAngle { value: 0 }

    // ось, вокруг которой происходит вращение
    property vector3d rotationAxis: Qt.vector3d(0, 0, 1)

    // JointNode, который перехватывает события мыши
    property JointNode eventFilterJoint

    pickable: true
    rotation: quaternionFromAxisAndAngle(angle, rotationAxis)

    function quaternionFromAxisAndAngle(angle: real, axis: vector3d): quaternion {
        // const angleRad = angleDegrees * (Math.PI / 180)
        const normAxis = axis.normalized()
        const halfAngle = angle * 0.5
        return Qt.quaternion(
            Math.cos(halfAngle),
            normAxis.x * Math.sin(halfAngle),
            normAxis.y * Math.sin(halfAngle),
            normAxis.z * Math.sin(halfAngle)
        )
    }
    function rotate(angle) {
        quaternionFromAxisAndAngle(angle, rotationAxis)
    }
}
